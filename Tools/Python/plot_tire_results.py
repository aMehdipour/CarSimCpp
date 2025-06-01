import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
df = pd.read_csv('/home/amehdipour/Repos/CarSimCpp/tire_model_test_results.csv')

# Create figure with subplots
fig, axes = plt.subplots(2, 2, figsize=(12, 10))
fig.suptitle('Tire Model Test Results', fontsize=16)

# Get unique normal loads
normal_loads = df['NormalLoad_lbs'].unique()

# Plot 1: Fy vs Slip Angle for different normal loads (at zero slip ratio)
ax1 = axes[0, 0]
for load in normal_loads:
    data = df[(df['NormalLoad_lbs'] == load) & (abs(df['SlipRatio']) < 0.01)]
    ax1.plot(data['SlipAngle_deg'], data['Fy_N'], label=f'{load} lbs')
    ax1.scatter(data['SlipAngle_deg'], data['Fy_N'], s=10, alpha=0.5)  # Scatter points for clarity
ax1.set_xlabel('Slip Angle (deg)')
ax1.set_ylabel('Lateral Force Fy (N)')
ax1.set_title('Lateral Force vs Slip Angle (Slip Ratio = 0)')
ax1.grid(True)
ax1.legend()

# Plot 2: Fx vs Slip Ratio for different normal loads (at zero slip angle)
ax2 = axes[0, 1]
for load in normal_loads:
    data = df[(df['NormalLoad_lbs'] == load) & (abs(df['SlipAngle_deg']) < 0.1)]
    ax2.plot(data['SlipRatio'], data['Fx_N'], label=f'{load} lbs')
    ax2.scatter(data['SlipRatio'], data['Fx_N'], s=10, alpha=0.5)  # Scatter points for clarity
ax2.set_xlabel('Slip Ratio')
ax2.set_ylabel('Longitudinal Force Fx (N)')
ax2.set_title('Longitudinal Force vs Slip Ratio (Slip Angle = 0°)')
ax2.grid(True)
ax2.legend()

# Plot 3: 3D surface plot of Fy for a specific normal load
ax3 = axes[1, 0]
load = 150  # Choose 150 lbs for the 3D plot
data_3d = df[df['NormalLoad_lbs'] == load]
slip_angles = data_3d['SlipAngle_deg'].unique()
slip_ratios = data_3d['SlipRatio'].unique()
X, Y = np.meshgrid(slip_angles, slip_ratios)
Z = data_3d.pivot_table(values='Fy_N', index='SlipRatio', columns='SlipAngle_deg').values

contour = ax3.contourf(X, Y, Z, levels=20, cmap='viridis')
ax3.set_xlabel('Slip Angle (deg)')
ax3.set_ylabel('Slip Ratio')
ax3.set_title(f'Lateral Force Fy (N) at {load} lbs Normal Load')
fig.colorbar(contour, ax=ax3)

# Plot 4: 3D surface plot of Fx for a specific normal load
ax4 = axes[1, 1]
Z_fx = data_3d.pivot_table(values='Fx_N', index='SlipRatio', columns='SlipAngle_deg').values

contour_fx = ax4.contourf(X, Y, Z_fx, levels=20, cmap='plasma')
ax4.set_xlabel('Slip Angle (deg)')
ax4.set_ylabel('Slip Ratio')
ax4.set_title(f'Longitudinal Force Fx (N) at {load} lbs Normal Load')
fig.colorbar(contour_fx, ax=ax4)

plt.tight_layout()
plt.savefig("output_plot.png", dpi=300)
plt.show()

# Print some statistics
print("\nTire Model Test Statistics:")
print("-" * 50)
for load in normal_loads:
    data_load = df[df['NormalLoad_lbs'] == load]
    print(f"\nNormal Load: {load} lbs")
    print(f"  Max Fy: {data_load['Fy_N'].max():.2f} N")
    print(f"  Min Fy: {data_load['Fy_N'].min():.2f} N")
    print(f"  Max Fx: {data_load['Fx_N'].max():.2f} N")
    print(f"  Min Fx: {data_load['Fx_N'].min():.2f} N")