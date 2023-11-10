/*
 * Copyright (c) 2023, Rauli Laine
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#pragma once

#include <algorithm>

#include <glibmm.h>

class PassEntries : public Glib::Object
{
public:
  using container_type = std::vector<Glib::ustring>;

  explicit PassEntries();

  static Glib::RefPtr<PassEntries> create();

  inline const container_type& entries() const
  {
    return m_container;
  }

  inline bool has(const Glib::ustring& entry) const
  {
    // Unfortunately Glib::ustring isn't hashable so we cannot use hash sets
    // here for faster lookup.
    return std::find(
      std::begin(m_container),
      std::end(m_container),
      entry
    ) != std::end(m_container);
  }

  void select(const Glib::ustring& entry) const;

private:
  void init();

private:
  container_type m_container;
};
