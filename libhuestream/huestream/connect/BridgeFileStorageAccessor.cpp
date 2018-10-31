/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include <huestream/connect/BridgeFileStorageAccessor.h>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <cassert>

#include "support/crypto/Hash.h"
#include "support/crypto/AES256.h"

namespace huestream {
    constexpr static auto PARTIAL_ENCRYPTION_KEY = "EDK_BRIDGE_INFO_";

    BridgeFileStorageAccessor::BridgeFileStorageAccessor(const std::string &fileName, AppSettingsPtr appSettings, BridgeSettingsPtr bridgeSettings) :
            _fileName(fileName),
            _appSettings(std::move(appSettings)),
            _bridgeSettings(std::move(bridgeSettings)) {
    }

    void BridgeFileStorageAccessor::Load(BridgesLoadCallbackHandler cb) {
        auto loaded_data = std::make_shared<HueStreamData>(_bridgeSettings);
        auto result = OPERATION_FAILED;

        std::fstream file;
        file.open(_fileName.c_str(), std::fstream::in | std::fstream::binary);
        if (file.is_open()) {
            std::ostringstream contents;
            contents << file.rdbuf();
            file.close();

            auto decrypted_contents = decrypt_data(contents.str());

            // Check if decryption failed
            if (!decrypted_contents.empty() || contents.str().empty()) {
                loaded_data->DeserializeText(decrypted_contents);
                result = OPERATION_SUCCESS;
            }
        }

        cb(result, loaded_data);
    }

    void BridgeFileStorageAccessor::Save(HueStreamDataPtr bridges, BridgesSaveCallbackHandler cb) {
        std::ofstream file;
        file.open(_fileName.c_str(), std::fstream::out | std::fstream::trunc | std::fstream::binary);
        if (file.is_open()) {
            auto data = bridges->SerializeText();
            auto encrypted_data = encrypt_data(data);

            assert(data.empty() || !encrypted_data.empty());

            file << encrypted_data;
            file.close();


            cb(OPERATION_SUCCESS);
        } else {
            cb(OPERATION_FAILED);
        }
    }

    std::string BridgeFileStorageAccessor::get_encryption_key_hash() {
        if (_appSettings == nullptr || _appSettings->GetStorageEncryptionKey().empty()) {
            return "";
        }

        return support::Hash::sha256(PARTIAL_ENCRYPTION_KEY + _appSettings->GetStorageEncryptionKey() + std::string("\0", 1));
    }

    std::string BridgeFileStorageAccessor::encrypt_data(const std::string& data) {
        auto encryptionKey = get_encryption_key_hash();

        if (encryptionKey.empty()) {
            return data;
        }

        return support::AES256::encrypt(encryptionKey, data);
    }

    std::string BridgeFileStorageAccessor::decrypt_data(const std::string& data) {
        auto encryptionKey = get_encryption_key_hash();

        if (encryptionKey.empty()) {
            return data;
        }

        return support::AES256::decrypt(encryptionKey, data);
    }
}  // namespace huestream
