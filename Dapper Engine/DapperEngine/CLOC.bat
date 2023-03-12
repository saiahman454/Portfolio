pushd
cd .\Source
..\cloc-1.84 --by-file --include-ext=cpp --csv --quiet --out=..\CLOC_OUTPUT.CSV .\
popd
