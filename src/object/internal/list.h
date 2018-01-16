/*
 * list.h
 * Copyright (C) 2010-2017 Belledonne Communications SARL
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

#ifndef _L_LIST_H_
#define _L_LIST_H_

#include "linphone/utils/general.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

/*
 * Internal compile time list.
 */
namespace Private {
	template<typename...>
	struct List {};

	template<typename Head, typename... Tail>
	struct List<Head, Tail...> {
		typedef Head Car;
		typedef List<Tail...> Cdr;
	};

	template<typename, typename>
	struct ListAppend;

	template<typename... L1, typename... L2>
	struct ListAppend<List<L1...>, List<L2...>> {
		typedef List<L1..., L2...> Value;
	};

	template<typename L, int N>
	struct ListBuilder {
		typedef typename ListAppend<
			List<typename L::Car>,
			typename ListBuilder<typename L::Cdr, N - 1>::Value
		>::Value Value;
	};

	template<typename L>
	struct ListBuilder<L, 0> {
		typedef List<> Value;
	};
}

LINPHONE_END_NAMESPACE

#endif // ifndef _L_LIST_H_