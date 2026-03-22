import matplotlib.pyplot as plt
import pandas as pd
from pathlib import Path

def plot_solutions(target_directory : str = ""):

    # Set the base directory 
    base_dir = Path(target_directory) if target_directory else Path(__file__).parent

    base_dir = Path.cwd() 

    # Build absolute paths to the files
    exact_file =  "exact_soln0.dat"
    numerical_file =  "soln0.dat"

    # Load exact data
    df_exact = pd.read_csv(exact_file, delim_whitespace=True, header=None, comment="#")
    df_exact = df_exact.apply(pd.to_numeric, errors='coerce')
    df_exact = df_exact.dropna()

    # Load Numerical Data
    df_numerical = pd.read_csv(numerical_file, delim_whitespace=True, header=None, comment="#")
    df_numerical = df_numerical.apply(pd.to_numeric, errors='coerce')
    df_numerical = df_numerical.dropna()

    # Extract x and y columns
    x_exact = df_exact[0].values
    y_exact = df_exact[1].values

    x_num = df_numerical[0].values
    y_num = df_numerical[1].values

    # Create the plot
    plt.figure(figsize=(8,5))

    # Plot exact solution
    plt.plot(x_exact, y_exact, label="Exact Solution", color="blue", linewidth=2)

    # Plot numerical solution
    plt.plot(x_num, y_num, label="Numerical Solution", color="red", linewidth=2)

    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Comparison: Exact vs Numerical Solution")
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    plot_solutions()
