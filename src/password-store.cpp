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
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>

#if defined(_WIN32)
# include <windows.h>
#else
# include <pwd.h>
# include <unistd.h>
#endif

#include "./password-store.hpp"

static std::optional<std::filesystem::path>
get_home_dir()
{
  const auto home = std::getenv("HOME");

  if (!home)
  {
#if defined(_WIN32)
    const auto userprofile = std::getenv("USERPROFILE");

    if (!userprofile)
    {
      const auto homedrive = std::getenv("HOMEDRIVE");
      const auto homepath = std::getenv("HOMEPATH");

      if (homedrive && *homedrive && homepath && *homepath)
      {
        return std::filesystem::path(homedrive) / homepath;
      }
    } else {
      return userprofile;
    }
#else
    const auto pw = ::getpwuid(::getuid());

    if (pw)
    {
      return pw->pw_dir;
    }
#endif

    return std::nullopt;
  }

  return home;
}

static std::optional<std::filesystem::path>
get_password_store_dir()
{
  std::filesystem::path store_dir;

  if (const auto custom_path = std::getenv("PASSWORD_STORE_DIR"))
  {
    store_dir = custom_path;
  }
  else if (const auto home_dir = get_home_dir())
  {
    store_dir = *home_dir / ".password-store";
  }
  if (std::filesystem::is_directory(store_dir))
  {
    return store_dir;
  }

  return std::nullopt;
}

PasswordStore::PasswordStore()
{
  reload();
}

void
PasswordStore::reload()
{
  using std::filesystem::recursive_directory_iterator;

  if (const auto store_dir = get_password_store_dir())
  {
    const auto store_path = store_dir->native();

    m_container.clear();
    for (const auto& file : recursive_directory_iterator(*store_dir))
    {
      if (file.is_regular_file() && file.path().extension() == ".gpg")
      {
        // We have a matching entry, but it's an absolute path. Lets tweak it
        // a little bit.
        auto path = file.path().native();

        // Remove the password store directory from the entry.
        path = path.substr(store_path.length() + 1);
        // Remove the ".gpg" extension from the entry.
        path = path.substr(0, path.length() - 4);
        m_container.push_back(path);
      }
    }
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
