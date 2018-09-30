#!/usr/bin/env python

import argparse
import numpy as np

parser = argparse.ArgumentParser(description='generate mac file with band spectrum')
parser.add_argument("macfile", help = "output mac file")
parser.add_argument("-a", dest = "polangle", help = "polarization angle, 0-180. < 0 means no pol", type = float, default = 0)
parser.add_argument("-t", dest = "theta", help = "incident angle: theta", type = float, default = 0)
parser.add_argument("-p", dest = "phi", help = "incident angle: phi", type = float, default = 0)
parser.add_argument("-A", dest = "alpha", help = "alpha parameter of band spectrum", type = float, default = -1)
parser.add_argument("-B", dest = "beta", help = "beta parameter of band spectrum", type = float, default = -2.5)
parser.add_argument("-E", dest = "epeak", help = "Epeak parameter of band spectrum", type = float, default = 200)
args = parser.parse_args()

def rad_to_deg(angle):
    return angle / np.pi * 180.0

def deg_to_rad(angle):
    return angle / 180.0 * np.pi

def rotY(vec3, angle):
    roty_mat = [[np.cos(angle), 0, np.sin(angle)], [0, 1, 0], [-np.sin(angle), 0, np.cos(angle)]]
    return np.dot(roty_mat, vec3)

def rotZ(vec3, angle):
    rotz_mat = [[np.cos(angle), -np.sin(angle), 0], [np.sin(angle), np.cos(angle), 0], [0, 0, 1]]
    return np.dot(rotz_mat, vec3)

def rotV(vec3, theta, phi):
    return rotZ(rotY(vec3, theta), phi)

def band_fun(e, epeak, alpha, beta):
    if (e < (alpha - beta) * epeak / (2 + alpha)):
        return (e/100.0)**alpha * np.exp(-e * (2 + alpha) / epeak)
    else:
        return (e/100.0)**beta * np.exp(beta - alpha) * ((alpha - beta) * epeak / 100 / (2 + alpha))**(alpha - beta)

energy_min, energy_max = 5, 1005
energy_hist = []
for e in xrange(energy_min, energy_max, 1):
    energy_hist.append([float(e + 1) / 1000.0, band_fun(float(e + 0.5), args.epeak, args.alpha, args.beta)])

if args.polangle < 0:
    pol_vec = [0, 0, 0]
else:
    pol_vec = rotV(rotZ([1, 0, 0], deg_to_rad(args.polangle)), deg_to_rad(args.theta), deg_to_rad(args.phi))

direction_vec = rotV([0, 0, -1], deg_to_rad(args.theta), deg_to_rad(args.phi))
plane_rot1 = rotV([1, 0, 0], deg_to_rad(args.theta), deg_to_rad(args.phi))
plane_rot2 = rotV([0, 1, 0], deg_to_rad(args.theta), deg_to_rad(args.phi))
plane_center = rotV([0, 0, 4], deg_to_rad(args.theta), deg_to_rad(args.phi))
plane_radius = 1.0 # meters

with open(args.macfile, 'w') as f:
    f.write('##############################################\n')
    f.write('# polangle = %f\n' % args.polangle)
    f.write('# theta = %f\n' % args.theta)
    f.write('# phi = %f\n' % args.phi)
    f.write('# alpha = %f\n' % args.alpha)
    f.write('# beta = %f\n' % args.beta)
    f.write('# epeak = %f\n' % args.epeak)
    f.write('##############################################\n')
    f.write('\n')
    f.write('/gps/pos/type Plane\n')
    f.write('/gps/pos/shape Circle\n')
    f.write('/gps/pos/centre %.9f %.9f %.9f m\n' % tuple(plane_center))
    f.write('/gps/pos/radius %.9f m\n' % plane_radius)
    f.write('/gps/pos/rot1 %.9f %.9f %.9f\n' % tuple(plane_rot1))
    f.write('/gps/pos/rot2 %.9f %.9f %.9f\n' % tuple(plane_rot2))
    f.write('/gps/direction %.9f %.9f %.9f\n' % tuple(direction_vec))
    f.write('\n')
    f.write('/gps/particle gamma\n')
    f.write('/gps/polarization %.9f %.9f %.9f\n' % tuple(pol_vec))
    f.write('/gps/ene/type Arb\n')
    f.write('/gps/hist/type arb\n')
    f.write('/gps/ene/min %f MeV\n' % (energy_min / 1000.0))
    f.write('/gps/ene/max %f MeV\n' % (energy_max / 1000.0))
    f.write('/gps/hist/point %e %e\n' % (energy_min / 1000.0, 1.0))
    for x in energy_hist:
        f.write('/gps/hist/point %e %e\n' % tuple(x))
    f.write('/gps/hist/inter Log\n')


