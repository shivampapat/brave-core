/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/federated/covariate_logs_util.h"

#include "base/strings/string_number_conversions.h"
#include "base/time/time.h"

namespace ads {

std::string ToString(const int64_t value) {
  return base::NumberToString(value);
}

std::string ToString(const bool value) {
  return value ? "true" : "false";
}

std::string ToString(const base::Time& time) {
  if (time.is_null())
    return {};
  return base::NumberToString(time.ToDoubleT());
}

}  // namespace ads
