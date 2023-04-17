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
ruff python-stubs --fix --line-length=100 --extend-select=PYI,N,Q,D --ignore=PYI021,D1,D203,D205,D212,D401

echo
echo Run autopep8
# --recursive does not work with ".pyi" files, so we have to feed files individually
autopep8 $(git ls-files 'python-stubs/**.py*') --in-place --recursive --aggressive --aggressive --aggressive --max-line-length=100

echo
echo Run add-trailing-comma
# This has been requested to Ruff: https://github.com/charliermarsh/ruff/issues/3713
add-trailing-comma $(git ls-files 'python-stubs/**.py*') --py36-plus

echo
echo Run mypy
mypy python-stubs --strict --python-version=3.8

# You *can* install the python wrapper using pip, but it is recommended to install node directly instead
echo
echo Run Pyright
npx pyright@1.1.300 python-stubs --pythonversion=3.8.5
