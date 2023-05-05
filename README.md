# twineq

A project in modern C++ that calculates an approximate solution of the equilibrium equation in the case of a three-parameter closure.

## Installation

```bash
git clone git@github.com:sermir2003/low_esotericism.git
cd low_esotericism
mkdir build-directory
cd build-directory
cmake ..
make
```

## Use

Biological and technical data are entered using json files. To create such a file, enter
```bash
./twineq newtask
```
to create a file with default name,  or
```bash
./twineq newtask research_name
```
if you want to specify a research name and it would be `research_name.json`.

Enter the task data

### Input

#### Kernels

currently only Gauss kernels are supported, to specify such a kernel, enter
The standart deviations for the death and birth kernels

```json
{
  "kernel": {
    "type": "Gauss",
    "sigma m": 0.15,
    "sigma w": 0.15,
    "limit m": -1,
    "limit w": -1
  }
}
```

where sigma w and sigma m are standard deviations for the death and birth kernels, and limit w and limit m --- limits of the kernels.

The kernels in this case are the following
$$m(x)=\begin{cases}
    \frac{1}{\text{sigma_m}\sqrt{2\pi}}e^{-\frac{1}{2}\left(\frac{x^2}{\text{sigma_m}^2}\right)} & x\leqslant \text{limit_m}, \\\
    0, & x> \text{limit_m}
\end{cases}$$

You can set the parameters to -1 if you want the kernel to have no limits and the kernel to look like this:

$$m(x)=\frac{1}{\text{sigma_m}\sqrt{2\pi}}e^{-\frac{1}{2}\left(\frac{x^2}{\text{sigma_m}^2}\right)}$$

#### Other parameters

| Parameter               | Description                                    | Type             |
|-------------------------|------------------------------------------------|------------------|
| b                       | Birth rating                                   | Real             |
| d                       | Environment death rating                       | Real             |
| d'                      | Competition rating                             | Real             |
| Grid count              | Number of calculation points                   | Positive integer |
| Iteration count         | Number of iterations of the algorithm          | Positive integer |
| Integration area radius | The rounding boundary of the integral `[-r,r]` | Real             |
| alpha, beta, gamma      | Parameters of three-parameter closure          | String           |

To start the calculations, enter
```bash
./twineq solve
```
to use data from a file with a default name,  or
```bash
./twineq solve research_name.json
```
if you want to specify a research name.

### Output

Upon completion of calculations on the input data from the file with name `research_name.json`, two files will be 
created in the same folder with it: `research_name_N.txt` and `research_name_C.csv`.
File `research_name_N.txt` would contain only one number: the first spatial moment.
File `research_name_C.csv` would contain a line graph of the second moment in the form of pairs of coordinates x,y.
The first line contains only the string "x,y", the subsequent ones contain the coordinates of the points.

To then build a line graph in the form of an image, you need to run a special python script in the project directory:
```bash
python3 plot_maker.py research_name
```
to build a graph image for the data from file`research_name_C.csv`.

## License

The program is licensed under the [MIT License](https://opensource.org/licenses/MIT)

The program contains the [nlohmann/json](https://github.com/nlohmann/json) library which is licensed under the [MIT License](https://opensource.org/licenses/MIT).