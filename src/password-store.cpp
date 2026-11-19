/*
 * Copyright (c) 2023-2026, Rauli Laine
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
#include <algorithm>
#include <iostream>
#include <optional>

#include "./password-store.hpp"

static std::optional<std::string>
get_password_store_dir()
{
  std::string store_dir = Glib::getenv("PASSWORD_STORE_DIR");

  if (store_dir.empty())
  {
    store_dir = Glib::build_filename(Glib::get_home_dir(), ".password-store");
  }
  if (Glib::file_test(store_dir, Glib::FileTest::IS_DIR))
  {
    return store_dir;
  }

  return std::nullopt;
}

static void
collect_entries(
  const std::string& dir_path,
  const std::string& relative_prefix,
  PasswordStore::container_type& entries
)
{
  Glib::Dir dir(dir_path);

  for (const auto& name : dir)
  {
    const auto full_path = Glib::build_filename(dir_path, name);

    if (Glib::file_test(full_path, Glib::FileTest::IS_DIR))
    {
      const auto next_prefix = relative_prefix.empty()
        ? name
        : Glib::build_filename(relative_prefix, name);

      collect_entries(full_path, next_prefix, entries);
    }
    else if (
      Glib::file_test(full_path, Glib::FileTest::IS_REGULAR) &&
      Glib::str_has_suffix(name, ".gpg")
    )
    {
      auto entry = relative_prefix.empty()
        ? name
        : Glib::build_filename(relative_prefix, name);

      // Remove the ".gpg" extension from the entry.
      entry.resize(entry.size() - 4);
      entries.push_back(Glib::filename_to_utf8(entry));
    }
  }
}

PasswordStore::PasswordStore()
{
  reload();
}

void
PasswordStore::reload()
{
  if (const auto store_dir = get_password_store_dir())
  {
    m_container.clear();
    collect_entries(*store_dir, {}, m_container);
    std::sort(std::begin(m_container), std::end(m_container));
  } else {
    std::cerr << "Unable to determine location of passwords." << std::endl;
  }
}

void
PasswordStore::select(const_reference entry)
{
  if (has(entry))
  {
#if defined(_WIN32)
    ::_execlp
#else
    ::execlp
#endif
    (
      "pass",
      "pass",
      "show",
      "-c",
      entry.c_str(),
      nullptr
    );
  }
}
