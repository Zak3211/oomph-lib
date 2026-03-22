import matplotlib.pyplot as plt
import pandas as pd
from pathlib import Path
from mpl_toolkits.mplot3d import Axes3D 

def plot_solutions(df_numerical):
    x_num = df_numerical[0].values
    y_num = df_numerical[1].values
    v1 = df_numerical[2].values
    v2 = df_numerical[3].values
    pressure = df_numerical[4].values

    # Create 3D figure
    fig = plt.figure(figsize=(10, 6))
    ax = fig.add_subplot(111, projection="3d")

    # Numerical solution (just mesh, no faces)
    ax.plot_trisurf(
        x_num, y_num, pressure,
        facecolors='red',                
        linewidth=0.5
    )

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    ax.set_title("Navier-Stokes Solution")

    plt.show()

import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

def plot_solutions_quiver_plot(df_numerical):
    x = df_numerical[0].values
    y = df_numerical[1].values
    u = df_numerical[2].values
    v = df_numerical[3].values

    # Structured grid
    xi = np.linspace(x.min(), x.max(), 200)
    yi = np.linspace(y.min(), y.max(), 200)
    Xi, Yi = np.meshgrid(xi, yi)

    # Interpolate velocities
    Ui = griddata((x, y), u, (Xi, Yi), method="cubic")
    Vi = griddata((x, y), v, (Xi, Yi), method="cubic")

    plt.figure(figsize=(6, 6))
    plt.streamplot(
        Xi, Yi, Ui, Vi,
        density=1.5,
        linewidth=1,
        arrowsize=1
    )

    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Velocity streamlines")
    plt.axis("equal")
    plt.show()



if __name__ == "__main__":

    numerical_file =  "RESLT/soln0.dat"

    df_numerical = pd.read_csv(
        numerical_file,
        delim_whitespace=True,
        header=None,
        comment="#",
        engine="python",      # <-- CRITICAL
        names=[0, 1, 2, 3, 4] # <-- FORCE 5 columns
    )

    # Convert to numeric
    df_numerical = df_numerical.apply(pd.to_numeric, errors="coerce")

    # Drop rows with fewer than 5 values
    df_numerical = df_numerical.dropna(subset=[0, 1, 2, 3, 4])
    
    plot_solutions_quiver_plot(df_numerical)
