/*
 * Copyright (c) 2023-2024, Rauli Laine
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

#include "./password-store.hpp"

class EntryListColumns : public Gtk::TreeModel::ColumnRecord
{
public:
  explicit EntryListColumns();

  inline const Gtk::TreeModelColumn<Glib::ustring>& entry_column() const
  {
    return m_entry_column;
  }

private:
  Gtk::TreeModelColumn<Glib::ustring> m_entry_column;
};

class EntryList : public Gtk::Bin
{
public:
  explicit EntryList(const std::shared_ptr<PasswordStore>& store);

  inline void set_filter(const Glib::ustring& filter)
  {
    m_filter = filter;
    m_tree_model_filter->refilter();
  }

  void reload();

protected:
  void on_row_activated(
    const Gtk::TreeModel::Path& path,
    Gtk::TreeViewColumn* column
  );

private:
  std::shared_ptr<PasswordStore> m_store;
  Gtk::ScrolledWindow m_scrolled_window;
  Gtk::TreeView m_tree_view;
  EntryListColumns m_columns;
  Glib::RefPtr<Gtk::ListStore> m_tree_model;
  Glib::RefPtr<Gtk::TreeModelFilter> m_tree_model_filter;
  Glib::ustring m_filter;
};
