# NOTICE

Pangate — Mists of Pandaria 5.4.8 (build 18414) server core with a playerbots module.

## License

This repository is distributed under the **GNU General Public License, version 3,
or (at your option) any later version** (**GPL-3.0-or-later**).
The full license text is in [COPYING.md](COPYING.md).

Individual source files carry a `SPDX-License-Identifier: GPL-3.0-or-later` tag
and the GPL notice. Third-party components keep their own licenses — see the
`LICENSE` / `COPYING` files inside `dep/` and `contrib/`, and the headers of the
individual files. Eluna (`src/server/game/LuaEngine`) is licensed GPL-3.0 only
and is left untouched by this project's licensing.

## License transition

Releases of this repository published **before 2026-11-23** (branch `playerbot`,
all commits up to and including `68634b87`) were distributed under the
GNU General Public License, version 2, or (at your option) any later version
(**GPL-2.0-or-later**).

From the relicensing commit (2026-11-23) onward, the project as a whole is
distributed under **GPL-3.0-or-later**.

This upgrade is permitted by GPLv2 section 9 ("or any later version") because
every inherited file grants "either version 2 of the License, or (at your
option) any later version"; no contributor permission is required. It is also
required for internal consistency, because this tree already contains
components that are GPLv3-only or GPLv2-incompatible:

- Eluna Lua engine (`src/server/game/LuaEngine`) — GPL-3.0
- OpenSSL 3.x (Apache-2.0) — compatible with GPLv3, incompatible with GPLv2
- gSOAP 2.8.117 (`dep/gsoap`) — GPL-2.0-or-later, fine either way

Older tags and published artefacts are not relicensed retroactively; they remain
available under GPL-2.0-or-later.

## Upstream

This project is derived from work by TrinityCore, Project SkyFire, Legends of
Azeroth (Pandaria Core lineage) and AzerothCore (mod-playerbots). Copyright
information is listed in [THANKS.md](THANKS.md). Note that code from
GPL-3.0-only projects (for example SkyFire_548) may be merged into this tree
under GPLv3 terms; the reverse — sending GPLv3 code back to a GPLv2-only
upstream — is not permitted.
