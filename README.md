# Umap

🗺️ [Umappp](https://github.com/LTLA/umappp) for Ruby

* Compatible with [yaumap](https://github.com/LTLA/yaumap/blob/master/R/umap_from_matrix.R)
* Support [Numo::NArray](https://github.com/ruby-numo/numo-narray)

🚧 alpha - 

## Installation

```
git clone https://github.com/kojix2/umap
cd umap
rake compile
rake install
```

* OpenMP is required for multithreading.

## Usage

```ruby
Umap.umap(pixels, nthreads: 4)
```

## Development

* [numo.hpp](https://github.com/ankane/numo.hpp)
* [rice](https://github.com/jasonroelofs/rice)

## Contributing

Welcome!

## License

* As for the code I wrote, it is MIT.
* For other codes, please check on your own.
