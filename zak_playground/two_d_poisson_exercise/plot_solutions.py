import matplotlib.pyplot as plt
import pandas as pd
from pathlib import Path
from mpl_toolkits.mplot3d import Axes3D 

def plot_solutions(target_directory : str = ""):

    # Set the base directory 
    base_dir = Path(target_directory) if target_directory else Path(__file__).parent

    base_dir = Path.cwd() 

    # Build absolute paths to the files
    exact_file =  "RESLT/exact_soln0.dat"
    numerical_file =  "RESLT/soln0.dat"

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
    z_exact = df_exact[2].values

    x_num = df_numerical[0].values
    y_num = df_numerical[1].values
    z_num = df_numerical[2].values

    # Create 3D figure
    fig = plt.figure(figsize=(10, 6))
    ax = fig.add_subplot(111, projection="3d")

    ax.plot_trisurf(
        x_exact, y_exact, z_exact,
        color="blue", alpha=0.7, linewidth=0.2
    )

    # Numerical solution (just mesh, no faces)
    ax.plot_trisurf(
        x_num, y_num, z_num,
        facecolors='none',       # No fill
        edgecolor='red',         # Show the edges
        linewidth=0.5
    )

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    ax.set_title("Exact vs Numerical Solution (3D)")

    plt.show()

if __name__ == "__main__":
    plot_solutions()
