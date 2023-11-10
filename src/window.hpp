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

#include "./entry-completion.hpp"
#include "./entry-list.hpp"
#include "./pass.hpp"

class Window : public Gtk::Window
{
public:
  explicit Window();

protected:
  bool on_key_press_event(GdkEventKey* event);
  void on_search_text_changed();
  void on_search_activated();

private:
  Glib::RefPtr<PassEntries> m_pass_entries;
  Gtk::Box m_box;
  Glib::RefPtr<EntryCompletion> m_search_entry_completion;
  Gtk::Box m_search_bar;
  Gtk::Entry m_search_entry;
  EntryList m_entry_list;
};
