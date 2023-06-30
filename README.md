# Blosc2 Btune

Btune is a dynamic plugin for Blosc2 that assists in finding the optimal combination of compression parameters. It works by training a neural network on your most representative datasets.

By default, this software uses a genetic algorithm to test different combinations of compression parameters that meet your requirements for both compression ratio and speed for every chunk in the dataset. It assigns a score to each combination and, after a number of iterations, the software stops and uses the best score (minimal value) found for the rest of the dataset. For a more graphical explanation, visit https://btune.blosc.org.

The process of finding optimal compression parameters in Blosc2 can be slow because of the large number of combinations of compression parameters (codec, compression level, filter, split mode, number of threads, etc.). This can require a significant amount of trial and error to find the best combinations. However, you can significantly accelerate this process by training a neural network on your own datasets.

To begin the training process, provide your datasets to the Blosc Development Team. We will then perform the training and provide neural network models tailored to your needs, along with general tuning advice for Blosc2. In exchange, we request financial contributions to the project.

If you are interested, please contact the Blosc Development Team at contact@blosc.org.

## Install the Btune wheel

Btune uses a Python wheel for installation, but it can be used from any application that uses C-Blosc2, whether it is in C, Python, or any other language. Currently, only Linux and Mac installers are supported.

```shell
pip install blosc2-btune
```

Next, we will run an example for Python and then for C.
To do so, change your current directory to `examples` from this repository.

````shell
cd examples
````

## Using Btune from Python

To use Btune with Blosc2 in Python, set the `BTUNE_BALANCE` environment variable to a floating-point number between 0 (to optimize speed) and 1 (to optimize compression ratio). Additionally, you can use `BTUNE_PERF_MODE` to optimize compression, decompression, or to achieve a balance between the two by setting it to `COMP`, `DECOMP`, or `BALANCED`, respectively.

```shell
BTUNE_BALANCE=0.5 BTUNE_PERF_MODE=COMP python create_schunk.py
SChunk succesfully created!
```

This creates an empty SChunk on dosk and append chunks alternating arange data chunks 
with random data chunks.

You can set `BTUNE_TRACE=1` to see what Btune is doing.

```shell
BTUNE_BALANCE=0.5 BTUNE_PERF_MODE=COMP BTUNE_TRACE=1  python create_schunk.py 
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Btune version: 1.0.0
Performance Mode: COMP, Compression balance: 0.500000, Bandwidth: 20 GB/s
Behaviour: Waits - 0, Softs - 5, Hards - 11, Repeat Mode - STOP
TRACE: Environment variable BTUNE_MODELS_DIR is not defined
WARNING: Empty metadata, no inference performed
|    Codec   | Filter | Split | C.Level | Blocksize | Shufflesize | C.Threads | D.Threads |   Score   |  C.Ratio   |   Btune State   | Readapt | Winner
|        lz4 |      0 |     1 |       6 |         0 |           4 |        16 |        16 |  0.000284 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|        lz4 |      0 |     0 |       6 |         0 |           4 |        16 |        16 |   0.00054 |         2x |    CODEC_FILTER |    HARD | W
|        lz4 |      1 |     1 |       6 |         0 |           4 |        16 |        16 |  0.000139 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|        lz4 |      1 |     0 |       6 |         0 |           4 |        16 |        16 |  8.12e-05 |       103x |    CODEC_FILTER |    HARD | W
|        lz4 |      2 |     1 |       6 |         0 |           4 |        16 |        16 |  0.000349 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|        lz4 |      2 |     0 |       6 |         0 |           4 |        16 |        16 |  0.000185 |       198x |    CODEC_FILTER |    HARD | W
|    blosclz |      0 |     1 |       6 |         0 |           4 |        16 |        16 |  4.99e-05 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|    blosclz |      0 |     0 |       6 |         0 |           4 |        16 |        16 |  0.000985 |      1.99x |    CODEC_FILTER |    HARD | -
|    blosclz |      1 |     1 |       6 |         0 |           4 |        16 |        16 |  0.000101 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|    blosclz |      1 |     0 |       6 |         0 |           4 |        16 |        16 |  0.000113 |      59.2x |    CODEC_FILTER |    HARD | -
SChunk succesfully created!
```

You can see in the column `Winner` if the combination is a winner (`W`), it does not improve
the previous winner (`-`) or it is a special value chunk meaning that it is really easy to 
compress no matter the compression parameters (`S`) so Btune cannot determine whether
this is a winner or not. 

The Blosc Development Team offers a service in which Btune uses neural network models trained specifically for your data to determine the optimal combination of codecs and filters. To use these models, set `BTUNE_MODELS_DIR` to the directory containing the model files after the Blosc Development Team has completed training. Btune will then automatically use the trained model.

To determine the number of chunks for performing inference, use `BTUNE_USE_INFERENCE`. If set to -1, it performs inference on all chunks. If set to a number greater than 0, it performs inference on this number of chunks and then tweaks parameters for the rest of the chunks. If set to 0, it does not perform inference at all. The default is -1.

```shell
BTUNE_BALANCE=0.5 BTUNE_PERF_MODE=COMP BTUNE_TRACE=1  BTUNE_MODELS_DIR=./models/ BTUNE_USE_INFERENCE=3 python create_schunk.py
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Btune version: 1.0.0
Performance Mode: COMP, Compression balance: 0.500000, Bandwidth: 20 GB/s
Behaviour: Waits - 0, Softs - 5, Hards - 11, Repeat Mode - STOP
INFO: Model files found in the './models//' directory
TRACE: time load model: 0.000091
TRACE: Inference category=6 codec=1 filter=2 clevel=5 splitmode=2 time entropy=0.001048 inference=0.000015
|    Codec   | Filter | Split | C.Level | Blocksize | Shufflesize | C.Threads | D.Threads |   Score   |  C.Ratio   |   Btune State   | Readapt | Winner
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |  0.000489 |   2.5e+04x |    CODEC_FILTER |    HARD | S
TRACE: Inference category=7 codec=1 filter=34 clevel=5 splitmode=2 time entropy=0.000252 inference=0.000003
|        lz4 |     34 |     1 |       5 |         0 |           4 |        16 |        16 |   0.00011 |       133x |    CODEC_FILTER |    HARD | W
TRACE: Inference category=6 codec=1 filter=2 clevel=5 splitmode=2 time entropy=0.000095 inference=0.000002
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |  0.000123 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |   8.4e-05 |       184x |    CODEC_FILTER |    HARD | W
|        lz4 |      2 |     0 |       5 |         0 |           4 |        16 |        16 |  0.000117 |   2.5e+04x |    CODEC_FILTER |    HARD | S
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |   0.00013 |       159x |    THREADS_COMP |    HARD | -
|        lz4 |      2 |     1 |       5 |         0 |           4 |        15 |        16 |  0.000473 |   2.5e+04x |    THREADS_COMP |    HARD | S
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |  0.000401 |       152x |          CLEVEL |    HARD | -
|        lz4 |      2 |     1 |       6 |         0 |           4 |        16 |        16 |   0.00013 |   2.5e+04x |          CLEVEL |    SOFT | S
|        lz4 |      2 |     1 |       5 |         0 |           4 |        16 |        16 |  0.000134 |      67.3x |          CLEVEL |    SOFT | -
SChunk succesfully created!
```

Using trained models leads to significantly better performance scores, as demonstrated by the balance between compression speed and compression ratio. Moreover, the process of finding the best combination is much faster with trained models.

## Using Btune from C

You can also use Btune from C. Similar to the Python examples above, you can activate it by setting the `BTUNE_BALANCE` environment variable. Alternatively, you can set the `tuner_id` in the compression parameters, also known as `cparams`, to the value of `BLOSC_BTUNE`. This will use the default Btune configuration. However, running Btune from C offers the advantage of tuning more parameters based on your interests:

```
    // compression params
    blosc2_cparams cparams = BLOSC2_CPARAMS_DEFAULTS;
    cparams.nthreads = 16; // Btune may lower this
    cparams.tuner_id = BLOSC_BTUNE;
    
    // Btune config parameters
    btune_config btune_config = BTUNE_CONFIG_DEFAULTS;
    btune_config.perf_mode = BTUNE_PERF_COMP; // You can choose BTUNE_PERF_COMP, BTUNE_PERF_DECOMP or BTUNE_PERF_BALANCED
    btune_config.comp_balance = .5; // Equivalent to BTUNE_BALANCE
    btune_config.use_inference = 2; // Equivalent to BTUNE_USE_INFERENCE
    btune_config.models_dir = "../models/"; // Equivalent to BTUNE_MODELS_DIR
    btune_config.behaviour.nwaits_before_readapt = 1;       // Number of waits before a readapt
    btune_config.behaviour.nsofts_before_hard = 3;          // Number of soft readapts before a hard readapt
    btune_config.behaviour.nhards_before_stop = 10;         // Number of hard readapts before stoping
    btune_config.behaviour.repeat_mode = BTUNE_REPEAT_ALL;  // Repeat all the initial readaptions (BTUNE_REPEAT_ALL), 
                                                            // only soft readaptions (BTUNE_REPEAT_SOFT)
                                                            // or stop improving (BTUNE_STOP)
    // Set the personalized Btune configuration
    cparams.tuner_params = &btune_config;

    // Create super chunk
    blosc2_dparams dparams = BLOSC2_DPARAMS_DEFAULTS;
    dparams.nthreads = 1;
    blosc2_storage storage = {
        .cparams=&cparams,
        .dparams=&dparams,
        .contiguous=true,
        .urlpath=(char*)out_fname
    };
    blosc2_schunk* schunk_out = blosc2_schunk_new(&storage);
```

See the full example in `examples/btune_example.c`.
