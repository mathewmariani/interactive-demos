# interactive-demos

[![Build](/../../actions/workflows/main.yml/badge.svg)](/../../actions/workflows/main.yml)

Interactive demos inspired by Amit Patel from [RedBlobGames](https://www.redblobgames.com/).

## How to Build

This project is built using Emscripten, and it is expected that `$EMSDK` exists on the `PATH`.

```
# builds emscripten module
> git clone https://github.com/mathewmariani/interactive-demos
> cd interactive-demos
> cmake --preset Release
> cmake --build --preset Release

# dev. environment
> npm install
> npm run dev
```

## License

This project is free software; you can redistribute it and/or modify it under the terms of the MIT license.
See [LICENSE](LICENSE) for details.