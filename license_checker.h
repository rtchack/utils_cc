/**
 * Created on: 6/27/17
 *     Author: xing
 */

#ifndef BASE_DEV_LICENSE_CHECKER_H
#define BASE_DEV_LICENSE_CHECKER_H

#include "json/json.hpp"

#include "botan/pubkey.h"
#include "botan/rsa.h"
#include "botan/auto_rng.h"
#include "botan/base64.h"

#include "curl_wrapper/connection.h"

#include "base/common.h"
#include "base/file_wrapper.h"
#include "base/helper.h"
#include "base/console.h"


namespace base{

	/**
	 * Should be called at the most beginning
	 */
	class LicenseChecker final{
	public:

		using json = ::nlohmann::json;

		LicenseChecker(const std::string &email,
		               const std::string &password,
		               const std::vector<uint8_t> &info_to_sign,
		               const std::string &rsa_pub_key_pem,
		               const std::string &cert_path,
		               const std::string &server_uri,
		               const std::string &action = "/instant/create",
		               const std::string &digest = "EMSA3(SHA-256)"):
				email{email},
				password{password},
				info_to_sign{info_to_sign},
				pub_key{Botan::X509::load_key(std::vector<uint8_t>{
						rsa_pub_key_pem.begin(), rsa_pub_key_pem.end()})},
				cert_path{cert_path},
				server_uri{server_uri},
				action{action},
				digest{digest} {
			{
				auto fl = fopen(cert_path.c_str(), "rb");
				unless(fl){
					std::cout << "Could not open "
					          << cert_path
					          << ", try to apply from license center"
					          << std::endl;
					BASE_RAISE_VERBOSE_UNLESS(ApplyCert(), "Server " << server_uri << action)
					fl = fopen(cert_path.c_str(), "rb");
					BASE_RAISE_VERBOSE_UNLESS(fl, "Reopen " << cert_path)
				}
				fclose(fl);
			}

			RestClient::init();
			BASE_RAISE_UNLESS(Verify())
			RestClient::disable();
		}

		LicenseChecker(const std::vector<uint8_t> &info_to_sign,
		               const std::string &rsa_pub_key_pem,
		               const std::string &cert_path,
		               const std::string &server_uri,
		               const std::string &action = "/instant/create",
		               const std::string &digest = "EMSA3(SHA-256)"):
				LicenseChecker{"", "", info_to_sign, rsa_pub_key_pem, cert_path, server_uri, action, digest} {}


	private:
		BASE_DISALLOW_COPY_AND_ASSIGN(LicenseChecker)

		const std::string Data2Send() {
			hashed_info = Botan::base64_encode(
					info_to_sign.data(), info_to_sign.size());

			unless(email.size() && password.size()){
				std::cout << "Your email for license: ";
				getline(std::cin, email);
				std::cout << "Password: ";
				{
					NoConsoleEcho no_echo{};
					getline(std::cin, password);
				}
			}

			std::cout << "Visiting server..." << std::endl;

			const json j{{"email",    email},
			             {"password", password},
			             {"data",     hashed_info},
			             {"remark",   "From License_Client_Mocker"}};
			const json j_final{{"inf", j.dump()}};

			std::stringstream str;
			str << j_final;
			return str.str();
		}

		bool ApplyCert() {
			RestClient::Connection conn{server_uri};
			conn.SetTimeout(8);
			RestClient::HeaderFields header_fields{};
			const static auto mime = "application/json";
			header_fields["Accept"] = mime;
			conn.SetHeaders(header_fields);
			conn.AppendHeader("Content-Type", mime);
			const auto data = Data2Send();
			const auto res = conn.post(action, data);

			unless(res.code >= 200 && res.code < 300){
				std::cout << "Error " << res.code << ".\n"
				          << (res.body.size() > 128 ?
				              res.body.substr(0, 64) + "\n..." : res.body)
				          << std::endl;
				const std::string tmp_file_name{"sys.inf"};
				{
					FileWrapper tmp_file{tmp_file_name, "wb"};
					tmp_file.Write(hashed_info);
				}
				std::cout << "Unable to visit " << server_uri
				          << "? Contact us with following string:\n"
				          << hashed_info << std::endl
				          << "Or send file '" << tmp_file_name << "' to us directly."
				          << std::endl << std::endl;
				return false;
			}

			base::FileWrapper out_file{cert_path, "wb"};
			const auto body = json::parse(res.body);
			const auto cert = body["cert"];
			out_file.Write(cert);
			return true;
		}

		std::string ReadCert() {
			base::FileWrapper file{cert_path, "rb"};
			return file.Read();
		}

		bool Verify() {
			const auto cert = ReadCert();
			auto signature = Botan::base64_decode(cert);
			Botan::PK_Verifier verifier{*pub_key, digest};
			verifier.update(info_to_sign);
			return verifier.check_signature(signature);
		}


		std::string email;
		std::string password;
		const std::vector<uint8_t> info_to_sign;
		std::unique_ptr<Botan::Public_Key> pub_key;
		const std::string cert_path;
		const std::string server_uri;
		const std::string action;
		const std::string digest;
		std::string hashed_info{};
	};

}

#endif //BASE_DEV_LICENSE_CHECKER_H
