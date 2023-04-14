# Run `pip install -r requirements-tests.txt` to install dependencies first

# PYI:  flake8-pyi
# N:    pep8-naming
# Q:    flake8-quotes
# D:    pydocstyle
# D1:     pydocstyle: Missing doctring
# PYI021: flake8-pyi: docstring-in-stub
# D203:   pydocstyle: one-blank-line-before-class
# D205:   pydocstyle: blank-line-after-summary
# D212:   pydocstyle: multi-line-summary-first-line
# D401:   pydocstyle: non-imperative-mood
echo
echo Run Ruff
ruff python-stubs --fix --extend-select=PYI,N,Q,D --ignore=PYI021,D1,D203,D205,D212,D401

echo
echo Run Black
black python-stubs --line-length=100

echo
echo Run mypy
mypy python-stubs --strict --python-version=3.8

# You *can* install the python wrapper using pip, but it is recommended to install node directly instead
echo
echo Run Pyright
npx pyright@1.1.300 python-stubs --pythonversion=3.8.5
