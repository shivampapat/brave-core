/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/log_entries/locale_country_at_time_of_serving_covariate_log_entry.h"

#include <cstdint>

#include "bat/ads/ads_client.h"
#include "bat/ads/internal/federated/covariate_logs_util.h"
#include "bat/ads/pref_names.h"
#include "brave/components/l10n/browser/locale_helper.h"
#include "brave/components/l10n/common/locale_util.h"

namespace ads {

LocaleCountryAtTimeOfServingCovariateLogEntry::
    LocaleCountryAtTimeOfServingCovariateLogEntry() = default;

LocaleCountryAtTimeOfServingCovariateLogEntry::
    ~LocaleCountryAtTimeOfServingCovariateLogEntry() = default;

mojom::DataType LocaleCountryAtTimeOfServingCovariateLogEntry::GetDataType()
    const {
  return mojom::DataType::kString;
}

mojom::CovariateType
LocaleCountryAtTimeOfServingCovariateLogEntry::GetCovariateType() const {
  return mojom::CovariateType::kLocaleCountryAtTimeOfServing;
}

std::string LocaleCountryAtTimeOfServingCovariateLogEntry::GetValue() const {
  const auto locale = brave_l10n::LocaleHelper::GetInstance()->GetLocale();
  const auto country_code = brave_l10n::GetCountryCode(locale);

  return country_code;
}

}  // namespace ads
