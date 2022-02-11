/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/solana_tx_service.h"

namespace brave_wallet {

SolanaTxService::SolanaTxService(TxServiceManager* tx_service_manager,
                                 JsonRpcService* json_rpc_service,
                                 KeyringService* keyring_service,
                                 PrefService* prefs)
    : TxService(tx_service_manager, json_rpc_service, keyring_service, prefs) {}

}  // namespace brave_wallet
