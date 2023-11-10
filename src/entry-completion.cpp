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
#include "./entry-completion.hpp"
#include "./pass.hpp"

EntryCompletionRecord::EntryCompletionRecord()
{
  add(m_text_column);
}

EntryCompletion::EntryCompletion(const Glib::RefPtr<PassEntries>& pass_entries)
  : m_completion_model(Gtk::ListStore::create(m_completion_record))
{
  set_model(m_completion_model);
  set_text_column(m_completion_record.text_column());
  for (const auto& entry : pass_entries->entries())
  {
    auto row = *(m_completion_model->append());

    row[m_completion_record.text_column()] = entry;
  }
}

Glib::RefPtr<EntryCompletion>
EntryCompletion::create(const Glib::RefPtr<PassEntries>& pass_entries)
{
  return Glib::RefPtr<EntryCompletion>(new EntryCompletion(pass_entries));
}
