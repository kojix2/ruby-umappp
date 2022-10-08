# This clones the umappp repository and obtains its dependencies.
# We vendor everything to avoid problems with managing compile-
# time dependencies in parallel with the R package ecosystem.

set -e
set -u

# Pulling down the dependencies.
if [ ! -e temp ]
then
    git clone https://github.com/LTLA/umappp temp
    cd temp 
else
    cd temp
    git checkout master
    git pull
fi
git checkout 46e169b8df80725abca76e8007840ec17c54c819
cmake -S . -B build -DBUILD_TESTING=OFF

# Cleaing out what was there before.
rm -rf ../include
mkdir ../include

# Copying over the headers.
cp -r include/umappp ../include/umappp

for x in aarand kmeans knncolle annoy irlba
do
    cp -r build/_deps/${x}-src/include/${x} ../include/${x}
done

cp -r build/_deps/eigen-src/Eigen ../include/Eigen
cp -r build/_deps/hnswlib-src/hnswlib/ ../include/hnswlib
cp -r build/_deps/powerit-src/include/powerit ../include/powerit

rm -rf ../../vendor
cp -r ../include ../../vendor