/*
 * encryption-chat-message-modifier.cpp
 * Copyright (C) 2010-2018 Belledonne Communications SARL
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "linphone/core.h"

#include "chat/chat-message/chat-message.h"
#include "chat/chat-room/abstract-chat-room.h"
#include "core/core.h"
#include "c-wrapper/c-wrapper.h"
#include "lime-backwards-compatible.h"

// =============================================================================

using namespace std;

LINPHONE_BEGIN_NAMESPACE

ChatMessageModifier::Result LimeBackwardsCompatible::processOutgoingMessage(
    const shared_ptr<ChatMessage> &message,
    int &errorCode
) {
    shared_ptr<AbstractChatRoom> chatRoom = message->getChatRoom();
    LinphoneImEncryptionEngine *imee = linphone_core_get_im_encryption_engine(chatRoom->getCore()->getCCore());
	if (!imee)
		return ChatMessageModifier::Result::Skipped;

	LinphoneImEncryptionEngineCbsOutgoingMessageCb cbProcessOutgoingMessage =
		linphone_im_encryption_engine_cbs_get_process_outgoing_message(
			linphone_im_encryption_engine_get_callbacks(imee)
		);

	if (!cbProcessOutgoingMessage)
		return ChatMessageModifier::Result::Skipped;

    errorCode = cbProcessOutgoingMessage(imee, L_GET_C_BACK_PTR(chatRoom), L_GET_C_BACK_PTR(message));
	if (errorCode == -1)
		return ChatMessageModifier::Result::Skipped;

	if (errorCode != 0 && errorCode != 1) {
		return ChatMessageModifier::Result::Error;
	}

	if (errorCode == 1)
		return ChatMessageModifier::Result::Suspended;

	return ChatMessageModifier::Result::Done;
}

ChatMessageModifier::Result LimeBackwardsCompatible::processIncomingMessage(
    const shared_ptr<ChatMessage> &message,
    int &errorCode
) {
    shared_ptr<AbstractChatRoom> chatRoom = message->getChatRoom();
    LinphoneImEncryptionEngine *imee = linphone_core_get_im_encryption_engine(chatRoom->getCore()->getCCore());
	if (!imee)
		return ChatMessageModifier::Result::Skipped;

	LinphoneImEncryptionEngineCbsIncomingMessageCb cbProcessIncomingMessage =
		linphone_im_encryption_engine_cbs_get_process_incoming_message(
			linphone_im_encryption_engine_get_callbacks(imee)
		);

	if (!cbProcessIncomingMessage)
		return ChatMessageModifier::Result::Skipped;

    errorCode = cbProcessIncomingMessage(imee, L_GET_C_BACK_PTR(chatRoom), L_GET_C_BACK_PTR(message));
	if (errorCode == -1)
		return ChatMessageModifier::Result::Skipped;

	if (errorCode != 0 && errorCode != 1) {
		return ChatMessageModifier::Result::Error;
	}

	if (errorCode == 1)
		return ChatMessageModifier::Result::Suspended;

	return ChatMessageModifier::Result::Done;
}

bool LimeBackwardsCompatible::encryptionEnabledForFileTransferCb(
    const shared_ptr<AbstractChatRoom> &chatRoom
) {
    // Backwards compatible methods
    return false;
}

void LimeBackwardsCompatible::generateFileTransferKeyCb(
    const shared_ptr<AbstractChatRoom> &chatRoom
) {
    // Backwards compatible methods
}

int LimeBackwardsCompatible::downloadingFileCb(
    const shared_ptr<ChatMessage> &message,
    size_t *offset,
    const uint8_t *buffer,
    size_t size,
    uint8_t *decrypted_buffer
) {
    // Backwards compatible methods
    return 0;
}

int LimeBackwardsCompatible::uploadingFileCb(
    const shared_ptr<ChatMessage> &message,
    size_t *offset,
    const uint8_t *buffer,
    size_t *size,
    uint8_t *encrypted_buffer
) {
    // Backwards compatible methods
    return 0;
}

LINPHONE_END_NAMESPACE