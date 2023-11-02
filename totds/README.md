# Trials of the dark sea

A game written in C++ for the [WASM-4](https://wasm4.org) fantasy console.

## Building

Build the cart by running:

```shell
make
```

Then run it with:

```shell
w4 run build/cart.wasm
```

For more info about setting up WASM-4, see the [quickstart guide](https://wasm4.org/docs/getting-started/setup?code-lang=cpp#quickstart).

## Links

- [Documentation](https://wasm4.org/docs): Learn more about WASM-4.
- [Snake Tutorial](https://wasm4.org/docs/tutorials/snake/goal): Learn how to build a complete game
  with a step-by-step tutorial.
- [GitHub](https://github.com/aduros/wasm4): Submit an issue or PR. Contributions are welcome!


## The part the game's programmer wrote

Made for The [WASM4Jam2](https://itch.io/jam/wasm4-v2)
Play it at https://ericvilcu.itch.io/trials-of-the-dark-sea

We may have little memory, few colors and a small screen, but you know what we DO have? CPU speed.
I am HEAVILY abusing that and this game may straight up not work on devices with bad CPUs.

Also, please disregard the excessively goofy project structure.

Note: wasm-opt was applied with O3 instead of Oz so it may run faster.
