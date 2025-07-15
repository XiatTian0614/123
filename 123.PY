import os, sys, csv
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter1d
from scipy.interpolate import interp1d
from vedo import Mesh, Plotter, Points
import vtk

##############################################################################
# 1. 讀取 STL
##############################################################################
stl_path = "林光照.stl"
mesh     = Mesh(stl_path).c("lightgray")
centroid = mesh.center_of_mass()
clicked  = []
plotter  = Plotter(title="平面截面擷取", axes=1, bg="white")
marker_radius = 12

##############################################################################
# 1-1 交線工具函式
##############################################################################
def intersect_plane(origin, normal):
    plane = vtk.vtkPlane(); plane.SetOrigin(*origin); plane.SetNormal(*normal)
    cutter = vtk.vtkCutter(); cutter.SetInputData(mesh.polydata()); cutter.SetCutFunction(plane); cutter.Update()
    stripper = vtk.vtkStripper(); stripper.SetInputConnection(cutter.GetOutputPort()); stripper.Update()
    output = stripper.GetOutput()
    return [np.array([output.GetCell(i).GetPoints().GetPoint(j)
                      for j in range(output.GetCell(i).GetPoints().GetNumberOfPoints())])
            for i in range(output.GetNumberOfCells())]

def resample_polyline(pts, step=0.01):
    res = [pts[0]]
    for i in range(1, len(pts)):
        p0, p1 = np.array(res[-1]), np.array(pts[i])
        v = p1 - p0; d = np.linalg.norm(v)
        while d >= step:
            new_pt = p0 + v * (step / d)
            res.append(new_pt.tolist())
            p0, v, d = new_pt, p1 - new_pt, np.linalg.norm(p1 - new_pt)
    if not np.allclose(res[-1], pts[-1]):
        res.append(pts[-1].tolist())
    return res

##############################################################################
# 2. 曲率分析工具
##############################################################################
def resample_equal_spacing(pts, step=0.05):
    d = np.linalg.norm(np.diff(pts, axis=0), axis=1)
    cumlen = np.hstack(([0], np.cumsum(d)))
    interp = interp1d(cumlen, pts, axis=0, kind='linear')
    new_len = np.arange(0, cumlen[-1], step)
    return interp(new_len)

def curvature(pts):
    k = [0.0]
    for i in range(1, len(pts) - 1):
        p0, p1, p2 = pts[i-1], pts[i], pts[i+1]
        a,b,c = np.linalg.norm(p1-p0), np.linalg.norm(p2-p1), np.linalg.norm(p2-p0)
        if a*b*c == 0: k.append(0.0); continue
        s = (a+b+c)/2
        area = np.sqrt(max(s*(s-a)*(s-b)*(s-c),0))
        sign = np.cross(np.append(p2-p1,0), np.append(p0-p1,0))[-1]
        k.append((4*area/(a*b*c)) if sign>=0 else -(4*area/(a*b*c)))
    k.append(0.0)
    return np.array(k)

def plot_curvature(csv_path="Model1.csv"):
    """回傳 (gm3D, cej3D, gm2D, cej2D)"""
    df = pd.read_csv(csv_path, header=None)
    pts3 = df.values
    origin = pts3.mean(axis=0)
    _,_,Vt = np.linalg.svd(pts3 - origin)
    basis2d = Vt[:2]                      # 兩條主成分為平面軸
    if np.cross(basis2d[0], basis2d[1])[2] < 0:
        basis2d[1] *= -1
    pts2 = (pts3 - origin) @ basis2d.T    # 投影到 2D
    pts2 = resample_equal_spacing(pts2, 0.05)
    pts2 = gaussian_filter1d(pts2, 1.2, axis=0)
    curv  = gaussian_filter1d(curvature(pts2), 2)

    # ---- GM 與 CEJ 索引 ----
    idx_gm  = int(np.argmax(curv))                            # 最凹
    gm2D    = pts2[idx_gm]
    gm_x    = gm2D[0]
    cand    = np.where((pts2[:,0] >= gm_x) & (curv < 0))[0]    # X 向右 & 曲率轉正
    idx_cej = int(cand[np.argmin(curv[cand])]) if len(cand) else idx_gm
    cej2D   = pts2[idx_cej]
    if pts2[idx_gm, 0] > pts2[idx_cej, 0]:
        pts2 = pts2[::-1]
        curv = curv[::-1]
        idx_gm = int(np.argmin(curv))
        gm_x = pts2[idx_gm, 0]
        cand = np.where((pts2[:,0] >= gm_x) & (curv > 0))[0]
        idx_cej = int(cand[np.argmax(curv[cand])]) if len(cand) else idx_gm

    gm2d, cej2d = pts2[idx_gm], pts2[idx_cej]
    dist  = np.linalg.norm(gm2d - cej2d)


    # ---- 可視化 ----
    cmin, cmax = curv.min(), curv.max()
    tan  = np.gradient(pts2, axis=0)
    norm = np.stack([-tan[:,1], tan[:,0]], axis=1)
    norm /= np.linalg.norm(norm, axis=1)[:,None]
    norm  = gaussian_filter1d(norm, 1, axis=0)
    length = np.abs(curv * 0.3)
    exp_l  = pts2 + norm * length[:,None]
    exp_r  = pts2 - norm * length[:,None]

    fig, ax = plt.subplots(figsize=(10,4))
    for i in range(len(pts2)-1):
        if curv[i] <= 0:  # 只畫正值曲率
            continue
        nv   = (curv[i]-cmin)/(cmax-cmin+1e-9)
        quad = np.array([pts2[i],
                        exp_r[i],
                        exp_r[i+1],
                        pts2[i+1]])
        ax.fill(quad[:,0], quad[:,1], color=plt.cm.Reds(nv))

    ax.plot(pts2[:,0], pts2[:,1], 'k', lw=1.5)
    ax.scatter(*gm2D, s=60, c='green', label='CEJ')
    ax.scatter(*cej2D, s=60, c='black', label='GM')
    ax.plot([gm2D[0], cej2D[0]], [gm2D[1], cej2D[1]], 'k--')
    ax.text(*(gm2D+cej2D)/2, f"距離: {dist:.2f}", ha='center')
    ax.legend(); ax.set_aspect('equal'); plt.tight_layout()
    plt.savefig("CurvatureMap.png", dpi=300); plt.close()
    print("✅ 圖已儲存：CurvatureMap.png")

    # ---- 轉回 3D 座標 ----
    gm3D  = origin + gm2D[0]*basis2d[0] + gm2D[1]*basis2d[1]
    cej3D = origin + cej2D[0]*basis2d[0] + cej2D[1]*basis2d[1]

    # ---- 列印 & 回傳 ----
    print(f"GM  3D 座標: {gm3D}")
    print(f"CEJ 3D 座標: {cej3D}\n")
    return cej3D, gm3D, gm2D, cej2D  # ✅ GM 黑點、CEJ 綠點


##############################################################################
# 3. 互動事件
##############################################################################
def on_click(evt):
    global plotter
    if evt.picked3d is None or len(clicked) >= 2:
        return
    pid = mesh.closest_point(evt.picked3d, return_point_id=True)
    if pid in clicked: return
    clicked.append(pid)
    plotter += Points(mesh.points[pid], r=marker_radius, c="red")

    if len(clicked) == 2:
        A, B = mesh.points[clicked[0]], mesh.points[clicked[1]]
        n = np.cross(B - A, centroid - A)
        if np.linalg.norm(n) == 0:
            print("⚠️ 三點共線，請重新點"); clicked.clear(); return
        n /= np.linalg.norm(n)
        polylines = intersect_plane(A, n)
        if not polylines:
            print("⚠️ 無交線"); clicked.clear(); return

        # 取通過 A、B 的最佳線
        best_line, best_val = None, np.inf
        for ln in polylines:
            dA = np.min(np.linalg.norm(ln - A, axis=1))
            dB = np.min(np.linalg.norm(ln - B, axis=1))
            if dA + dB < best_val:
                best_line, best_val = ln, dA + dB
        if best_line is None:
            print("⚠️ 找不到同線"); clicked.clear(); return

        # 切出 AB 之間的一段
        idxA = np.argmin(np.linalg.norm(best_line - A, axis=1))
        idxB = np.argmin(np.linalg.norm(best_line - B, axis=1))
        if idxA <= idxB:
            path_fwd = best_line[idxA:idxB+1]
            path_rev = np.vstack((best_line[idxB:], best_line[:idxA+1]))[::-1]
        else:
            path_fwd = np.vstack((best_line[idxA:], best_line[:idxB+1]))
            path_rev = best_line[idxB:idxA+1][::-1]
        path = path_fwd if len(path_fwd) <= len(path_rev) else path_rev

        # 等距取樣 & 輸出 CSV
        sampled = resample_polyline(path, step=0.01)
        with open("Model1.csv","w",newline="") as f:
            csv.writer(f).writerows(sampled)
        plotter += Points(sampled, r=4, c="black")
        print(f"✅ 已輸出 Model1.csv ({len(sampled)} 點)，開始曲率分析…")
        plotter.render()

        # ==== 曲率分析 ====
        gm3D, cej3D, _, _ = plot_curvature("Model1.csv")          ### ← 修改：接收 3D 座標
        # ---- 在 3D 模型標記 ----
        plotter += Points([gm3D],  r=18, c="black")               ### ← 新增：GM 黑點
        plotter += Points([cej3D], r=18, c="green")               ### ← 新增：CEJ 綠點
        plotter.render()

        clicked.clear()

def on_key(evt):
    if evt.keypress == "Escape":
        plotter.close(); sys.exit()

# ---------------------------------------------------------------------
plotter.add_callback("click", on_click)
plotter.add_callback("key",   on_key)
plotter.show(mesh, viewup="z", interactive=True)
