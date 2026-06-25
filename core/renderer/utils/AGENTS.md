# AGENTS.md

## Scope

This directory contains renderer-specific utility helpers such as renderer env/tracing helpers, diff utilities, devtool lifecycle state, and prop-bundle style writing helpers.

## Edit Rules

- Keep this directory dependency-light and utility-oriented. Do not move core DOM or pipeline ownership here.
- Diff and prop-bundle helpers are reused broadly; small changes can cause many downstream regressions.
- Harmony settings values can reach `LynxEnv::GetExternalEnv()` through
  `LynxTrailHub.setTrailMap()` by string key even when the key is not declared
  in `LynxTrailKey`. Libra values are different: Harmony filters Libra data
  through `Object.values(LynxTrailKey)` before forwarding it into the native
  trail map. When adding a new `LynxEnv::Key` that needs Libra control or typed
  Harmony trail access, also add the matching string value to Harmony's
  exported `LynxTrailKey` enum; locate it by searching for
  `export enum LynxTrailKey` if needed. If a new key is settings-only or
  local-only, make that intentional in the change or review notes.

## Common Regression Symptoms

- Devtool lifecycle state drifts from renderer behavior after helper changes.
- Diff or prop-bundle utilities regress multiple unrelated renderer tests at once.

## Validate

Use `lynx-cpp-test` and start with:

- `renderer_utils_unittests_exec`
