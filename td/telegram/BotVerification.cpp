//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "td/telegram/BotVerification.h"

#include "td/telegram/Td.h"
#include "td/telegram/UserManager.h"

namespace td {

BotVerification::BotVerification(telegram_api::object_ptr<telegram_api::botVerification> &&bot_verification) {
  if (bot_verification == nullptr) {
    return;
  }
  bot_user_id_ = UserId(bot_verification->bot_id_);
  settings_ = BotVerifierSettings(std::move(bot_verification));
}

unique_ptr<BotVerification> BotVerification::get_bot_verification(
    telegram_api::object_ptr<telegram_api::botVerification> &&bot_verification) {
  if (bot_verification == nullptr) {
    return nullptr;
  }
  auto result = td::make_unique<BotVerification>(std::move(bot_verification));
  if (!result->is_valid()) {
    LOG(ERROR) << "Receive invalid " << *result;
    return nullptr;
  }
  return result;
}

td_api::object_ptr<td_api::botVerification> BotVerification::get_bot_verification_object(Td *td) const {
  if (!is_valid()) {
    return nullptr;
  }
  return td_api::make_object<td_api::botVerification>(
      td->user_manager_->get_user_id_object(bot_user_id_, "botVerification"),
      settings_.get_bot_verification_parameters_object(td));
}

bool operator==(const BotVerification &lhs, const BotVerification &rhs) {
  return lhs.bot_user_id_ == rhs.bot_user_id_ && lhs.settings_ == rhs.settings_;
}

bool operator==(const unique_ptr<BotVerification> &lhs, const unique_ptr<BotVerification> &rhs) {
  if (lhs == nullptr) {
    return rhs == nullptr;
  }
  if (rhs == nullptr) {
    return false;
  }
  return *lhs == *rhs;
}

StringBuilder &operator<<(StringBuilder &string_builder, const BotVerification &bot_verification) {
  return string_builder << "verified by " << bot_verification.bot_user_id_ << " with " << bot_verification.settings_;
}

}  // namespace td