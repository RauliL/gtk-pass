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

#include <gtkmm.h>

#include "./pass.hpp"

class EntryCompletionRecord : public Gtk::TreeModel::ColumnRecord
{
public:
  explicit EntryCompletionRecord();

  inline const Gtk::TreeModelColumn<Glib::ustring>& text_column() const
  {
    return m_text_column;
  }

private:
  Gtk::TreeModelColumn<Glib::ustring> m_text_column;
};

class EntryCompletion : public Gtk::EntryCompletion
{
public:
  explicit EntryCompletion(const Glib::RefPtr<PassEntries>& pass_entries);

  static Glib::RefPtr<EntryCompletion> create(
    const Glib::RefPtr<PassEntries>& pass_entries
  );

private:
  EntryCompletionRecord m_completion_record;
  Glib::RefPtr<Gtk::ListStore> m_completion_model;
};
