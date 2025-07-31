import math

# ---------- Given / Assumed Inputs ----------
gamma = 1.4
R = 287                          # J/kg-K
cp = gamma * R / (gamma - 1)     # J/kg-K
eta_tt = 0.80                    # total-to-total efficiency
PR = 3.0                         # total pressure ratio
m_dot = 0.557171                 # kg/s
Tt2 = 300                        # K
Pt2 = 101325                        # Pa
slip = 0.815                     # Slip factor for 12 blades
phi = 0.12                       # Flow coefficient (Vz / U2)
hub_tip_ratio = 0.4              # r_hub / r_tip

# ---------- Step 1: Total Enthalpy Rise ----------
Tt3s = Tt2 * (PR)**((gamma - 1) / gamma)
delta_h_isentropic = cp * (Tt3s - Tt2)
delta_h_actual = delta_h_isentropic / eta_tt

# ---------- Step 2: Tip speed and flow velocity ----------
U2 = math.sqrt(delta_h_actual / slip)
Vz = phi * U2

# ---------- Step 3: Exit thermodynamics ----------
T3 = Tt2 + delta_h_actual / cp
rho3 = (Pt2 * PR) / (R * T3)

# ---------- Step 4: Required area from continuity ----------
A_required = m_dot / (rho3 * Vz)

# ---------- Step 5: Solve for tip radius using binary search ----------
def find_tip_radius(area_required, beta, tol=1e-6, max_iter_count=100):
    r_lo = 0.01  # 10 mm
    r_hi = 0.3   # 300 mm

    for _ in range(max_iter_count):
        r_mid = (r_lo + r_hi) / 2
        r_hub = beta * r_mid
        area_actual = math.pi * (r_mid**2 - r_hub**2)
        error = area_actual - area_required

        if abs(error) < tol:
            return r_mid
        elif error > 0:
            r_hi = r_mid
        else:
            r_lo = r_mid

    return r_mid  # best guess if no convergence

r_tip = find_tip_radius(A_required, hub_tip_ratio)
r_hub = hub_tip_ratio * r_tip
omega = U2 / r_tip
RPM = omega * 60 / (2 * math.pi)

# ---------- Output ----------
print("---- Centrifugal Compressor Output ----")
print(f"Mass Flow Rate: {m_dot:.4f} kg/s")
print(f"Total Pressure Ratio: {PR:.4f}")
print(f"Flow Coefficient (phi): {phi}")
print(f"Slip Factor: {slip}")
print(f"Exit Temperature: {T3:.4f} K")
print(f"Exit Density: {rho3:.4f} kg/m³")
print(f"Tip Speed (U2): {U2:.4f} m/s")
print(f"Axial Exit Velocity (Vz): {Vz:.4f} m/s")
print(f"Required Flow Area: {A_required*1e4:.4f} cm²")
print(f"Calculated Tip Radius: {r_tip:.6f} m")
print(f"Calculated Hub Radius: {r_hub:.6f} m")
print(f"Calculated RPM: {RPM:.0f} RPM")
