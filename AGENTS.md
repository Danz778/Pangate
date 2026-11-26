# AGENTS.md

MoP 5.4.8 (build 18414) C++ server core (worldserver/authserver) with the playerbots module. Not a
Node/Python project; there is no npm, pip, or lint/format tooling in the repo.

## Build

- Out-of-source only: CMake aborts on in-source builds (`CMAKE_DISABLE_IN_SOURCE_BUILD`).
- Linux, from the repo root:
  ```
  mkdir bin && cd bin
  cmake ../ -DTOOLS=1 -DELUNA=0 -DCMAKE_INSTALL_PREFIX=check_install
  make -j$(nproc) && make install
  ./check_install/bin/worldserver --version   # CI smoke check; also authserver --version
  ```
  Use `-DTOOLS=0` for a faster local build when the extractors are not needed (CI builds with `1`).
- Windows CI: `mkdir build && cd build && cmake .. -DTOOLS=ON -DELUNA=0 -A x64`, then
  `cmake --build . --config RelWithDebInfo`. Binaries land in `build/bin/RelWithDebInfo/`.
  `CMakeSettings.json` (VS 2019 generator) is the local Visual Studio config.
- `ELUNA` defaults to 1 in `cmake/options.cmake`; CI always passes `-DELUNA=0`. Match CI unless you need Eluna.
- Options that matter (`cmake/options.cmake`): `TOOLS` (map/vmap/mmap extractors, default 0),
  `PLAYERBOTS` (default 1), `SCRIPTS` (1), `SERVERS` (1), `UPDATER` (0), `WITH_WARNINGS`, `WITH_COREDEBUG`,
  `NOPCH` (hidden; disables precompiled headers).
- Toolchain: C++20 (`CMAKE_CXX_STANDARD 20`, no extensions). Linux CI uses GCC 13 and Clang 15 (the Clang
  job's `--version` check is commented out); Windows CI uses VS 2022, Boost 1.87 and OpenSSL 3.1.1. There is
  also a macOS arm64 build. `linux-x86_64.yml` adds `-DBUILD_DEPLOY=0` and `-fno-strict-aliasing`.
  The README lists MySQL 5.7 / 8.0-8.1 and CMake >= 3.27.2 (`CMakeLists.txt` itself requires 3.16).
- No automated test suite runs in CI. Verification = a clean build plus `--version` on both binaries.
  Behavioral testing of bots uses `contrib/headless-client/headless.py` against a running server; see its README.

## Repo layout

- `src/server/` (authserver, worldserver, game, scripts), `src/common/`, `src/tools/`, `src/updater/`
  (not built unless `UPDATER=1`). `dep/` holds bundled third-party libraries built as part of the tree.
- `modules/mod_playerbots/` is the playerbots module (`src/`: AI, Db, Factory, Manager, Scenario, strategy,
  Mgr, Travel, Utils). Config is `config/playerbots.conf.dist`.
- `modules/CMakeLists.txt` auto-discovers any `modules/<dir>` containing `.cpp`/`.h`, and drops
  `mod_playerbots` when `PLAYERBOTS=0`.
- `sql/updates/{world,characters,master}/` holds DB update files, named `YYYY_MM_DD_NN_<name>.sql`.
  Playerbots DB updates live under `modules/mod_playerbots/data/sql/`.
- `contrib/` (headless client, debugger, scripts converter, etc.) is tooling, not part of the server build.
- `doc/code_standards.md`: 4 spaces, no tabs, LF line endings only, no trailing whitespace, keep PRs to
  few squashed commits, no merge commits unless the PR is a merge.

## Gotchas

- **`modules/ModulesLoader.cpp` is generated (but tracked by git).** `modules/CMakeLists.txt` rewrites it from
  `ModulesLoader.cpp.in.cmake` on every configure (copy_if_different). Do not hand-edit it; change the
  module list through `modules/` directories or the template instead.
- **Upstream sync is automated.** `.github/workflows/upstream-sync.yml` (every 6 h) cherry-picks
  Legends-of-Azeroth `master` commits into `playerbot-beta` when they are "auto" per
  `.github/upstream-sync/upstream_sync.py`. Commits that need review block the run until ported by hand
  (`git cherry-pick -x`) or listed in `.github/upstream-sync/skip.txt`. Do not rewrite `playerbot-beta`
  history.
- **Branches:** the default branch is `playerbot`. The sync and pre-release target is `playerbot-beta`.
- **Database updates are applied by the worldserver** only when `Updates.EnableDatabases` in `worldserver.conf`
  enables them. `worldserver.conf.dist` sets it to `0` (disabled); `15` covers all four, including playerbots.
  Adding a `.sql` file to an update directory is the normal change; the sync workflow renames added files
  to today's date. Never ship a full `characters.sql`/`auth.sql`/`playerbots.sql` dump over an existing install.
- **Runtime data is not in the repo.** Map, vmap, and mmap data must be extracted from a 5.4.8 enUS client
  (see README §2). Bots need mmaps; generation takes hours. Without them the server runs but bots path poorly.
- **Config files:** copy `*.conf.dist` to `.conf` beside the binaries. The playerbots DB connection is set only
  in `worldserver.conf`. `SOAP.Enabled` is needed for GM-verb scripting.
- **Headless client credentials:** `contrib/headless-client/headless.local.json` holds account/password.
  It is not matched by the root `.gitignore`, so never stage or commit it, and do not read or print it. Environment variables `MOP_ACCOUNT` and `MOP_PASSWORD` are an alternative.
- Windows release binaries need `libmysql.dll`, `libcrypto-3-x64.dll`, and `libssl-3-x64.dll` copied beside
  them (see `windows-build-release.yml`).

## Working style

- Use modern C++ with RAII and smart pointers for new owning code; avoid owning raw pointers, leaks, and
  undefined behavior. Where existing engine APIs manage object lifetime, follow their ownership rules.
- Keep header/source structure tidy. Do not refactor unrelated legacy code.
- Read selectively: search file names or symbols first, then read only the relevant line ranges. Avoid
  rereading large files.
- Skip `build/`, `bin/`, `out/`, `.git/`, `.vs/`, raw assets, logs, and caches. Open `dep/` only when the task
  involves a bundled library.
- Keep explanations short, show targeted diffs, and do not paste back unchanged files.

## Before you finish

- Build with the CI flags for the platform you changed (`-DTOOLS=1 -DELUNA=0`), and run `--version` on both binaries.
- For SQL changes, check that the file applies to a clean DB and re-applies cleanly; the upstream sync
  gate re-runs new update files twice to enforce idempotence.
- Follow `doc/code_standards.md`. Check for trailing whitespace and CRLF before committing.
