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
#include "./entry-list.hpp"

EntryListColumns::EntryListColumns()
{
  add(m_entry_column);
}

EntryList::EntryList(const std::shared_ptr<PasswordStore>& store)
  : m_store(store)
  , m_tree_model(Gtk::ListStore::create(m_columns))
  , m_tree_model_filter(Gtk::TreeModelFilter::create(m_tree_model))
{
  set_margin_top(8);
  set_margin_bottom(8);
  set_margin_left(8);
  set_margin_right(8);

  m_tree_view.set_model(m_tree_model_filter);
  m_tree_view.set_headers_visible(false);
  m_tree_view.append_column("Entry", m_columns.entry_column());
  m_tree_view.signal_row_activated().connect(sigc::mem_fun(
    this,
    &EntryList::on_row_activated
  ));

  m_tree_model_filter->set_visible_func(
    [this](const Gtk::TreeModel::const_iterator& i) -> bool
    {
      if (m_filter.length() > 0 && i)
      {
        auto row = *i;
        Glib::ustring entry(row[m_columns.entry_column()]);

        // TODO: Replace with proper fuzzy search at some point.
        return entry.find(m_filter) != Glib::ustring::npos;
      }

      return true;
    }
  );

  m_scrolled_window.set_policy(
    Gtk::POLICY_AUTOMATIC,
    Gtk::POLICY_AUTOMATIC
  );
  m_scrolled_window.add(m_tree_view);
  add(m_scrolled_window);

  reload();
}

void
EntryList::reload()
{
  const auto& entry_column = m_columns.entry_column();

  m_tree_model->clear();
  for (const auto& entry : *m_store)
  {
    auto row = *(m_tree_model->append());

    row[entry_column] = entry;
  }
}

void
EntryList::on_row_activated(
  const Gtk::TreeModel::Path& path,
  Gtk::TreeViewColumn* column
)
{
  const auto iter = m_tree_model->get_iter(path);

  if (iter)
  {
    const auto& row = *iter;

    m_store->select(row[m_columns.entry_column()]);
  }
}
