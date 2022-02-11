/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/tx_service.h"

#include "base/check.h"

namespace brave_wallet {

TxService::TxService(TxServiceManager* tx_service_manager,
                     JsonRpcService* json_rpc_service,
                     KeyringService* keyring_service,
                     PrefService* prefs)
    : tx_service_manager_(tx_service_manager),
      json_rpc_service_(json_rpc_service),
      keyring_service_(keyring_service),
      prefs_(prefs) {
  DCHECK(tx_service_manager_);
  DCHECK(json_rpc_service_);
  DCHECK(keyring_service_);
}

}  // namespace brave_wallet
