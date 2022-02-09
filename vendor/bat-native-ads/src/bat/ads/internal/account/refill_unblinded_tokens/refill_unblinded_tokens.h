/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_H_

#include <string>
#include <vector>

#include "base/check_op.h"
#include "base/memory/weak_ptr.h"
#include "bat/ads/internal/account/refill_unblinded_tokens/refill_unblinded_tokens_delegate.h"
#include "bat/ads/internal/account/wallet/wallet_info.h"
#include "bat/ads/internal/backoff_timer.h"
#include "bat/ads/internal/privacy/tokens/token_aliases.h"
#include "bat/ads/internal/tokens/refill_unblinded_tokens/refill_unblinded_tokens_delegate.h"
#include "bat/ads/public/interfaces/ads.mojom.h"

namespace ads {

namespace privacy {
namespace cbr {
class TokenGeneratorInterface;
}  // namespace cbr
}  // namespace privacy

class RefillUnblindedTokens final {
 public:
  explicit RefillUnblindedTokens(
      privacy::cbr::TokenGeneratorInterface* token_generator);
  ~RefillUnblindedTokens();

  void set_delegate(RefillUnblindedTokensDelegate* delegate) {
    DCHECK_EQ(delegate_, nullptr);
    delegate_ = delegate;
  }

  void MaybeRefill(const WalletInfo& wallet);

 private:
  WalletInfo wallet_;

  std::string nonce_;

  privacy::cbr::TokenList tokens_;
  privacy::cbr::BlindedTokenList blinded_tokens_;

  void Refill();

  void RequestSignedTokens();
  void OnRequestSignedTokens(const mojom::UrlResponse& url_response);

  void GetSignedTokens();
  void OnGetSignedTokens(const mojom::UrlResponse& url_response);

  void OnDidRefillUnblindedTokens();

  void OnFailedToRefillUnblindedTokens(const bool should_retry);

  BackoffTimer retry_timer_;
  void Retry();
  void OnRetry();

  bool ShouldRefillUnblindedTokens() const;

  int CalculateAmountOfTokensToRefill() const;

  bool is_processing_ = false;

  privacy::cbr::TokenGeneratorInterface* token_generator_;  // NOT OWNED

  RefillUnblindedTokensDelegate* delegate_ = nullptr;

  base::WeakPtrFactory<RefillUnblindedTokens> weak_ptr_factory_{this};
};

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_H_
