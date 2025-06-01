import matplotlib.pyplot as plt
import numpy as np
import csv

# Read the CSV file
data = []
with open('tire_model_test_results.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        data.append({
            'NormalLoad_lbs': float(row['NormalLoad_lbs']),
            'SlipAngle_deg': float(row['SlipAngle_deg']),
            'SlipRatio': float(row['SlipRatio']),
            'Fx_N': float(row['Fx_N']),
            'Fy_N': float(row['Fy_N'])
        })

# Convert to numpy arrays for easier manipulation
normal_loads = sorted(set(d['NormalLoad_lbs'] for d in data))
slip_angles = sorted(set(d['SlipAngle_deg'] for d in data))
slip_ratios = sorted(set(d['SlipRatio'] for d in data))

# Create figure with subplots
fig, axes = plt.subplots(2, 3, figsize=(18, 10))
fig.suptitle('Tire Model Test Results', fontsize=16)

# Plot 1: Fy vs Slip Angle for different normal loads (at zero slip ratio)
ax1 = axes[0, 0]
for load in normal_loads:
    # Filter data for this load and zero slip ratio
    filtered_data = [d for d in data if d['NormalLoad_lbs'] == load and abs(d['SlipRatio']) < 0.01]
    x = [d['SlipAngle_deg'] for d in filtered_data]
    y = [d['Fy_N'] for d in filtered_data]
    ax1.plot(x, y, label=f'{load} lbs', marker='o', markersize=3)
ax1.set_xlabel('Slip Angle (deg)')
ax1.set_ylabel('Lateral Force Fy (N)')
ax1.set_title('Lateral Force vs Slip Angle (Slip Ratio = 0)')
ax1.grid(True)
ax1.legend()

# Plot 2: Fx vs Slip Ratio for different normal loads (at zero slip angle)
ax2 = axes[0, 1]
for load in normal_loads:
    # Filter for zero slip angle
    load_data = [d for d in data if d['NormalLoad_lbs'] == load and abs(d['SlipAngle_deg']) < 0.1]
    slip_ratios_for_load = [d['SlipRatio'] for d in load_data]
    fx_for_load = [d['Fx_N'] for d in load_data]

    # Sort by slip ratio
    sorted_pairs = sorted(zip(slip_ratios_for_load, fx_for_load))
    slip_ratios_sorted = [x[0] for x in sorted_pairs]
    fx_sorted = [x[1] for x in sorted_pairs]

    # Plot with markers to show the discontinuity
    ax2.plot(slip_ratios_sorted, fx_sorted, label=f'{load} lbs', marker='o', markersize=2, markevery=5)

ax2.set_xlabel('Slip Ratio')
ax2.set_ylabel('Longitudinal Force Fx (N)')
ax2.set_title('Longitudinal Force vs Slip Ratio (Slip Angle = 0°)')
ax2.grid(True)
ax2.legend()
ax2.axhline(y=0, color='k', linestyle='--', alpha=0.3)
ax2.axvline(x=0, color='k', linestyle='--', alpha=0.3)

# Plot 3: Contour plot of Fy for a specific normal load
ax3 = axes[0, 2]
load = 150  # Choose 150 lbs for the contour plot
# Create meshgrid
X = np.array(slip_angles)
Y = np.array(slip_ratios)
XX, YY = np.meshgrid(X, Y)
# Create Z matrix
Z = np.zeros_like(XX)
for i, sr in enumerate(slip_ratios):
    for j, sa in enumerate(slip_angles):
        # Find the matching data point
        for d in data:
            if (d['NormalLoad_lbs'] == load and
                abs(d['SlipRatio'] - sr) < 1e-6 and
                abs(d['SlipAngle_deg'] - sa) < 1e-6):
                Z[i, j] = d['Fy_N']
                break

contour = ax3.contourf(XX, YY, Z, levels=20, cmap='viridis')
ax3.set_xlabel('Slip Angle (deg)')
ax3.set_ylabel('Slip Ratio')
ax3.set_title(f'Lateral Force Fy (N) at {load} lbs Normal Load')
fig.colorbar(contour, ax=ax3)

# Plot 4: Contour plot of Fx for a specific normal load
ax4 = axes[1, 1]
# Create Z matrix for Fx
Z_fx = np.zeros_like(XX)
for i, sr in enumerate(slip_ratios):
    for j, sa in enumerate(slip_angles):
        # Find the matching data point
        for d in data:
            if (d['NormalLoad_lbs'] == load and
                abs(d['SlipRatio'] - sr) < 1e-6 and
                abs(d['SlipAngle_deg'] - sa) < 1e-6):
                Z_fx[i, j] = d['Fx_N']
                break

contour_fx = ax4.contourf(XX, YY, Z_fx, levels=20, cmap='plasma')
ax4.set_xlabel('Slip Angle (deg)')
ax4.set_ylabel('Slip Ratio')
ax4.set_title(f'Longitudinal Force Fx (N) at {load} lbs Normal Load')
fig.colorbar(contour_fx, ax=ax4)

# Plot 5: Zoomed view of Fx discontinuity around zero slip ratio
ax5 = axes[1, 2]
for load in normal_loads:
    # Filter for zero slip angle and small slip ratio range
    load_data = [d for d in data if d['NormalLoad_lbs'] == load and
                 abs(d['SlipAngle_deg']) < 0.1 and
                 -0.05 <= d['SlipRatio'] <= 0.05]
    slip_ratios_for_load = [d['SlipRatio'] for d in load_data]
    fx_for_load = [d['Fx_N'] for d in load_data]

    # Sort by slip ratio
    sorted_pairs = sorted(zip(slip_ratios_for_load, fx_for_load))
    slip_ratios_sorted = [x[0] for x in sorted_pairs]
    fx_sorted = [x[1] for x in sorted_pairs]

    # Plot with markers
    ax5.plot(slip_ratios_sorted, fx_sorted, label=f'{load} lbs', marker='o', markersize=3)

ax5.set_xlabel('Slip Ratio')
ax5.set_ylabel('Longitudinal Force Fx (N)')
ax5.set_title('Fx Discontinuity Detail (Slip Angle = 0°)')
ax5.set_xlim(-0.05, 0.05)
ax5.grid(True, alpha=0.3)
ax5.legend()
ax5.axhline(y=0, color='k', linestyle='--', alpha=0.5)
ax5.axvline(x=0, color='k', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.show()

# Print some statistics
print("\nTire Model Test Statistics:")
print("-" * 50)
for load in normal_loads:
    load_data = [d for d in data if d['NormalLoad_lbs'] == load]
    fy_values = [d['Fy_N'] for d in load_data]
    fx_values = [d['Fx_N'] for d in load_data]
    print(f"\nNormal Load: {load} lbs")
    print(f"  Max Fy: {max(fy_values):.2f} N")
    print(f"  Min Fy: {min(fy_values):.2f} N")
    print(f"  Max Fx: {max(fx_values):.2f} N")
    print(f"  Min Fx: {min(fx_values):.2f} N")