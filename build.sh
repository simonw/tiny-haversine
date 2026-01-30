#!/usr/bin/env bash
set -euo pipefail

PYTHON_VERSION="${PYTHON_VERSION:-3.13}"
PYODIDE_VERSION="${PYODIDE_VERSION:-0.29.3}"
EMSCRIPTEN_VERSION="${EMSCRIPTEN_VERSION:-4.0.9}"

XENV="${XENV:-$PWD/.pyodide/xbuildenv/$PYODIDE_VERSION}"
OUTDIR="${OUTDIR:-dist}"

echo "[tiny-haversine] Building native wheels with uv build..."
uv build

echo "[tiny-haversine] Python: $PYTHON_VERSION"
echo "[tiny-haversine] Pyodide: $PYODIDE_VERSION"
echo "[tiny-haversine] Emscripten: $EMSCRIPTEN_VERSION"
echo "[tiny-haversine] xbuildenv path: $XENV"
echo "[tiny-haversine] output dir: $OUTDIR"

uv run --python "$PYTHON_VERSION" --with pyodide-build bash -lc "
  set -euo pipefail

  pyodide xbuildenv install '$PYODIDE_VERSION' --path '$XENV'
  pyodide xbuildenv install-emscripten --version '$EMSCRIPTEN_VERSION' --path '$XENV'

  EMSDK_ENV=\"\$(find '$XENV' -name emsdk_env.sh -print -quit)\"
  if [ -z \"\$EMSDK_ENV\" ]; then
    echo 'Could not find emsdk_env.sh under $XENV' >&2
    exit 1
  fi

  # shellcheck disable=SC1090
  source \"\$EMSDK_ENV\"

  PYODIDE_XBUILDENV_PATH='$XENV' pyodide build -o '$OUTDIR'
"

echo "[tiny-haversine] Done. Wheels in: $OUTDIR"
