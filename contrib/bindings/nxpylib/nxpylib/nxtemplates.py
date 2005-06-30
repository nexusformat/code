from nexus import *
from nxtemplatecomponents import *

group_types = {}

NXaperture_attrs = {}
NXaperture_attrs['name'] = NXattr_type(name='name', info = 'Name of aperture', desc = '', minOccur = 1, enums = [])

NXaperture_elems = {}

horizontal_aperture_attrs = {}
horizontal_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXaperture_elems['horizontal_aperture'] = NXelem_type(name='horizontal_aperture', types = [5],  dims = [1], info = 'Horizontal aperture', desc = '', minOccur = 0, enums = [],  attrs = horizontal_aperture_attrs)

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXaperture_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

shape_attrs = {}
NXaperture_elems['shape'] = NXelem_type(name='shape', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"Rectangular"', '"Circular"', '"Elliptical"'],  attrs = shape_attrs)

vertical_aperture_attrs = {}
vertical_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXaperture_elems['vertical_aperture'] = NXelem_type(name='vertical_aperture', types = [5],  dims = [1], info = 'Vertical aperture', desc = '', minOccur = 0, enums = [],  attrs = vertical_aperture_attrs)

radius_attrs = {}
radius_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXaperture_elems['radius'] = NXelem_type(name='radius', types = [5],  dims = [1], info = 'Radius of aperture (if circular)', desc = '', minOccur = 0, enums = [],  attrs = radius_attrs)
NXaperture_groups = {}

group_types['NXaperture'] = NXgroup_type(nxclass = 'NXaperture', minOccur= 0, maxOccur= 0, attrs= NXaperture_attrs, elems= NXaperture_elems, groups= NXaperture_groups)


NXattenuator_attrs = {}
NXattenuator_attrs['name'] = NXattr_type(name='name', info = 'Name of attenuator', desc = '', minOccur = 1, enums = [])

NXattenuator_elems = {}

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXattenuator_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

attenuation_attrs = {}
NXattenuator_elems['attenuation'] = NXelem_type(name='attenuation', types = [5],  dims = [1], info = 'Attenuation factor at the nominal beam energy', desc = '', minOccur = 0, enums = [],  attrs = attenuation_attrs)

thickness_attrs = {}
thickness_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXattenuator_elems['thickness'] = NXelem_type(name='thickness', types = [5],  dims = [1], info = 'Thickness of attenuator along beam direction', desc = '', minOccur = 0, enums = [],  attrs = thickness_attrs)

type_attrs = {}
NXattenuator_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Type of attenuator, e.g. polythene', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

absorption_cross_section_attrs = {}
absorption_cross_section_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['barns'])
NXattenuator_elems['absorption_cross_section'] = NXelem_type(name='absorption_cross_section', types = [5],  dims = [1], info = 'Absorption cross section', desc = '', minOccur = 0, enums = [],  attrs = absorption_cross_section_attrs)

scattering_cross_section_attrs = {}
scattering_cross_section_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['barns'])
NXattenuator_elems['scattering_cross_section'] = NXelem_type(name='scattering_cross_section', types = [5],  dims = [1], info = 'Scattering cross section (coherent+incoherent)', desc = '', minOccur = 0, enums = [],  attrs = scattering_cross_section_attrs)
NXattenuator_groups = {}

group_types['NXattenuator'] = NXgroup_type(nxclass = 'NXattenuator', minOccur= 0, maxOccur= 0, attrs= NXattenuator_attrs, elems= NXattenuator_elems, groups= NXattenuator_groups)


NXbeam_attrs = {}
NXbeam_attrs['name'] = NXattr_type(name='name', info = 'Name of beam plane', desc = '', minOccur = 1, enums = [])

NXbeam_elems = {}

incident_polarization_attrs = {}
NXbeam_elems['incident_polarization'] = NXelem_type(name='incident_polarization', types = [5],  dims = [3, -1], info = 'Polarization vector on entering beamline component', desc = '', minOccur = 0, enums = [],  attrs = incident_polarization_attrs)

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXbeam_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

incident_wavelength_attrs = {}
incident_wavelength_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstroms'])
NXbeam_elems['incident_wavelength'] = NXelem_type(name='incident_wavelength', types = [5],  dims = [-1], info = 'Wavelength on entering beamline component', desc = '', minOccur = 0, enums = [],  attrs = incident_wavelength_attrs)

incident_energy_attrs = {}
incident_energy_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meV'])
NXbeam_elems['incident_energy'] = NXelem_type(name='incident_energy', types = [5],  dims = [-1], info = 'Energy on entering beamline component', desc = '', minOccur = 0, enums = [],  attrs = incident_energy_attrs)

final_polarization_attrs = {}
NXbeam_elems['final_polarization'] = NXelem_type(name='final_polarization', types = [5],  dims = [3, -1], info = 'Polarization vector  on leaving beamline component', desc = '', minOccur = 0, enums = [],  attrs = final_polarization_attrs)

incident_wavelength_spread_attrs = {}
incident_wavelength_spread_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstroms'])
NXbeam_elems['incident_wavelength_spread'] = NXelem_type(name='incident_wavelength_spread', types = [5],  dims = [-1], info = 'Wavelength spread FWHM on entering component', desc = '', minOccur = 0, enums = [],  attrs = incident_wavelength_spread_attrs)

final_wavelength_spread_attrs = {}
final_wavelength_spread_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstroms'])
NXbeam_elems['final_wavelength_spread'] = NXelem_type(name='final_wavelength_spread', types = [5],  dims = [-1], info = 'Wavelength spread FWHM of beam leaving this component', desc = '', minOccur = 0, enums = [],  attrs = final_wavelength_spread_attrs)

flux_attrs = {}
flux_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['s-1cm-2'])
NXbeam_elems['flux'] = NXelem_type(name='flux', types = [5],  dims = [-1], info = 'flux incident on beam plane area', desc = '', minOccur = 0, enums = [],  attrs = flux_attrs)

incident_beam_divergence_attrs = {}
incident_beam_divergence_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree'])
NXbeam_elems['incident_beam_divergence'] = NXelem_type(name='incident_beam_divergence', types = [5],  dims = [2, -1], info = 'Divergence of beam entering this component', desc = '', minOccur = 0, enums = [],  attrs = incident_beam_divergence_attrs)

final_wavelength_attrs = {}
NXbeam_elems['final_wavelength'] = NXelem_type(name='final_wavelength', types = [5],  dims = [-1], info = 'Wavelength on leaving beamline component', desc = '', minOccur = 0, enums = [],  attrs = final_wavelength_attrs)

energy_transfer_attrs = {}
energy_transfer_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meV'])
NXbeam_elems['energy_transfer'] = NXelem_type(name='energy_transfer', types = [5],  dims = [-1], info = 'Energy change caused by beamline component ', desc = '', minOccur = 0, enums = [],  attrs = energy_transfer_attrs)

final_beam_divergence_attrs = {}
final_beam_divergence_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degrees'])
NXbeam_elems['final_beam_divergence'] = NXelem_type(name='final_beam_divergence', types = [5],  dims = [2, -1], info = 'Divergence FWHM of beam leaving this component', desc = '', minOccur = 0, enums = [],  attrs = final_beam_divergence_attrs)

final_energy_attrs = {}
final_energy_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meV'])
NXbeam_elems['final_energy'] = NXelem_type(name='final_energy', types = [5],  dims = [-1], info = 'Energy on leaving beamline component', desc = '', minOccur = 0, enums = [],  attrs = final_energy_attrs)
NXbeam_groups = {}
NXbeam_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXbeam'] = NXgroup_type(nxclass = 'NXbeam', minOccur= 0, maxOccur= 0, attrs= NXbeam_attrs, elems= NXbeam_elems, groups= NXbeam_groups)


NXbeamstop_attrs = {}
NXbeamstop_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['beamstop_name'])

NXbeamstop_elems = {}

y_attrs = {}
y_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXbeamstop_elems['y'] = NXelem_type(name='y', types = [5],  dims = [1], info = 'y position of the beamstop in relation to the detector', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = y_attrs)

x_attrs = {}
x_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXbeamstop_elems['x'] = NXelem_type(name='x', types = [5],  dims = [1], info = 'x position of the beamstop in relation to the detector', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = x_attrs)

status_attrs = {}
NXbeamstop_elems['status'] = NXelem_type(name='status', types = [4],  dims = [-1], info = 'in|out', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = status_attrs)

description_attrs = {}
NXbeamstop_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'description of beamstop: circular | rectangular', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = description_attrs)

size_attrs = {}
size_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXbeamstop_elems['size'] = NXelem_type(name='size', types = [5],  dims = [1], info = 'size of beamstop', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = size_attrs)
NXbeamstop_groups = {}

group_types['NXbeamstop'] = NXgroup_type(nxclass = 'NXbeamstop', minOccur= 0, maxOccur= 0, attrs= NXbeamstop_attrs, elems= NXbeamstop_elems, groups= NXbeamstop_groups)


NXcrystal_attrs = {}
NXcrystal_attrs['name'] = NXattr_type(name='name', info = 'Name of crystal beamline component', desc = '', minOccur = 1, enums = [])

NXcrystal_elems = {}

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXcrystal_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance of chopper from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

lattice_parameter_attrs = {}
lattice_parameter_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstrom'])
NXcrystal_elems['lattice_parameter'] = NXelem_type(name='lattice_parameter', types = [5],  dims = [1], info = 'Lattice parameter of the nominal reflection', desc = '', minOccur = 0, enums = [],  attrs = lattice_parameter_attrs)

reflection_attrs = {}
NXcrystal_elems['reflection'] = NXelem_type(name='reflection', types = [24],  dims = [-1], info = '[hkl] values of nominal reflection', desc = '', minOccur = 0, enums = [],  attrs = reflection_attrs)

horizontal_aperture_attrs = {}
horizontal_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcrystal_elems['horizontal_aperture'] = NXelem_type(name='horizontal_aperture', types = [5],  dims = [1], info = 'Horizontal aperture, if rectangular', desc = '', minOccur = 0, enums = [],  attrs = horizontal_aperture_attrs)

wavelength_attrs = {}
wavelength_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstroms'])
NXcrystal_elems['wavelength'] = NXelem_type(name='wavelength', types = [5],  dims = [1], info = 'Optimum diffracted wavelength', desc = '', minOccur = 0, enums = [],  attrs = wavelength_attrs)

horizontal_curvature_attrs = {}
horizontal_curvature_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degrees'])
NXcrystal_elems['horizontal_curvature'] = NXelem_type(name='horizontal_curvature', types = [5],  dims = [1], info = 'Horizontal curvature of focusing crystal', desc = '', minOccur = 0, enums = [],  attrs = horizontal_curvature_attrs)

vertical_aperture_attrs = {}
vertical_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcrystal_elems['vertical_aperture'] = NXelem_type(name='vertical_aperture', types = [5],  dims = [1], info = 'Vertical aperture, if rectangular', desc = '', minOccur = 0, enums = [],  attrs = vertical_aperture_attrs)

vertical_curvature_attrs = {}
vertical_curvature_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degrees'])
NXcrystal_elems['vertical_curvature'] = NXelem_type(name='vertical_curvature', types = [5],  dims = [1], info = 'Vertical curvature of focusing crystal', desc = '', minOccur = 0, enums = [],  attrs = vertical_curvature_attrs)

energy_attrs = {}
energy_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meV'])
NXcrystal_elems['energy'] = NXelem_type(name='energy', types = [5],  dims = [1], info = 'Optimum diffracted energy', desc = '', minOccur = 0, enums = [],  attrs = energy_attrs)
NXcrystal_groups = {}

group_types['NXcrystal'] = NXgroup_type(nxclass = 'NXcrystal', minOccur= 0, maxOccur= 0, attrs= NXcrystal_attrs, elems= NXcrystal_elems, groups= NXcrystal_groups)


NXdata_attrs = {}
NXdata_attrs['name'] = NXattr_type(name='name', info = 'Name of data block', desc = '', minOccur = 1, enums = [])

NXdata_elems = {}

variable_attrs = {}
variable_attrs['long_name'] = NXattr_type(name='long_name', info = 'Axis label', desc = '', minOccur = 1, enums = [])
variable_attrs['distribution'] = NXattr_type(name='distribution', info = '', desc = '', minOccur = 1, enums = ['0', '1'])
variable_attrs['first_good'] = NXattr_type(name='first_good', info = 'Index of first good value', desc = '', minOccur = 1, enums = [])
variable_attrs['last_good'] = NXattr_type(name='last_good', info = 'Index of last good value', desc = '', minOccur = 1, enums = [])
NXdata_elems['variable'] = NXelem_type(name='variable', types = [5, 24],  dims = [-1], info = 'Dimension scale defining an axis of the data', desc = '', minOccur = 0, enums = [],  attrs = variable_attrs)

errors_attrs = {}
NXdata_elems['errors'] = NXelem_type(name='errors', types = [5],  dims = ['...'], info = 'Standard deviations of data values - the data array is identified by the attribute signal="1". This array must have the same dimensions as the data', desc = '', minOccur = 0, enums = [],  attrs = errors_attrs)

data_attrs = {}
data_attrs['long_name'] = NXattr_type(name='long_name', info = 'Title of data', desc = '', minOccur = 1, enums = [])
data_attrs['signal'] = NXattr_type(name='signal', info = '', desc = '', minOccur = 1, enums = ['1'])
data_attrs['axes'] = NXattr_type(name='axes', info = '', desc = '', minOccur = 1, enums = ['[...]'])
NXdata_elems['data'] = NXelem_type(name='data', types = [5, 24],  dims = ['...'], info = 'Data values', desc = '', minOccur = 0, enums = [],  attrs = data_attrs)

variable_errors_attrs = {}
NXdata_elems['variable_errors'] = NXelem_type(name='variable_errors', types = [5, 24],  dims = [-1], info = 'Errors associated with axis "variable"', desc = '', minOccur = 0, enums = [],  attrs = variable_errors_attrs)
NXdata_groups = {}

group_types['NXdata'] = NXgroup_type(nxclass = 'NXdata', minOccur= 0, maxOccur= 0, attrs= NXdata_attrs, elems= NXdata_elems, groups= NXdata_groups)


NXdetector_attrs = {}
NXdetector_attrs['name'] = NXattr_type(name='name', info = 'Name of detector bank', desc = '', minOccur = 1, enums = [])

NXdetector_elems = {}

azimuthal_angle_attrs = {}
azimuthal_angle_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree'])
NXdetector_elems['azimuthal_angle'] = NXelem_type(name='azimuthal_angle', types = [5],  dims = [-1], info = 'Azimuthal angle of detector element', desc = '', minOccur = 0, enums = [],  attrs = azimuthal_angle_attrs)

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meter'])
NXdetector_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [-1], info = 'Effective distance from scattering centre (secondary flight path)', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

solid_angle_attrs = {}
solid_angle_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['steradians'])
NXdetector_elems['solid_angle'] = NXelem_type(name='solid_angle', types = [5],  dims = [-1], info = 'Solid angle subtended by the detector at the sample', desc = '', minOccur = 0, enums = [],  attrs = solid_angle_attrs)

pixel_raster_attrs = {}
NXdetector_elems['pixel_raster'] = NXelem_type(name='pixel_raster', types = [4],  dims = [-1], info = 'numbering/labelling scheme for pixels: +x+y would mean number along the x axis in a positive direction and then move up y in a positive diection (so bottom right to top left as viewed from the moderator)', desc = '', minOccur = 0, enums = ['"+x+y"', '"+x-y"', '"-x+y"', '"-x-y"', '"+y+x"', '"+y-x"', '"-y+x"', '"-y-x"'],  attrs = pixel_raster_attrs)

description_attrs = {}
NXdetector_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'name/manufacturer/model/etc. information', desc = '', minOccur = 0, enums = [],  attrs = description_attrs)

slot_attrs = {}
slot_attrs['local_name'] = NXattr_type(name='local_name', info = 'Equivalent local term', desc = '', minOccur = 1, enums = [])
NXdetector_elems['slot'] = NXelem_type(name='slot', types = [24],  dims = [-1], info = 'Slot number of detector', desc = '', minOccur = 0, enums = [],  attrs = slot_attrs)

detection_gas_path_attrs = {}
detection_gas_path_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXdetector_elems['detection_gas_path'] = NXelem_type(name='detection_gas_path', types = [5],  dims = [1], info = 'maximum drift space dimension', desc = '', minOccur = 0, enums = [],  attrs = detection_gas_path_attrs)

input_attrs = {}
input_attrs['local_name'] = NXattr_type(name='local_name', info = 'Equivalent local term', desc = '', minOccur = 1, enums = [])
NXdetector_elems['input'] = NXelem_type(name='input', types = [24],  dims = [-1], info = 'Input number of detector', desc = '', minOccur = 0, enums = [],  attrs = input_attrs)

dead_time_attrs = {}
NXdetector_elems['dead_time'] = NXelem_type(name='dead_time', types = [5],  dims = [-1], info = 'Detector dead time', desc = '', minOccur = 0, enums = [],  attrs = dead_time_attrs)

crate_attrs = {}
crate_attrs['local_name'] = NXattr_type(name='local_name', info = 'Equivalent local term', desc = '', minOccur = 1, enums = [])
NXdetector_elems['crate'] = NXelem_type(name='crate', types = [24],  dims = [-1], info = 'Crate number of detector', desc = '', minOccur = 0, enums = [],  attrs = crate_attrs)

pixel_centre_attrs = {}
NXdetector_elems['pixel_centre'] = NXelem_type(name='pixel_centre', types = [5],  dims = [-1], info = 'Coordinates of the pixel of the direct beam centre (1D or 2D)', desc = '', minOccur = 0, enums = [],  attrs = pixel_centre_attrs)

time_of_flight_attrs = {}
time_of_flight_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['microsecond'])
NXdetector_elems['time_of_flight'] = NXelem_type(name='time_of_flight', types = [5],  dims = [-1], info = 'Neutron time-of-flight', desc = '', minOccur = 0, enums = [],  attrs = time_of_flight_attrs)

polar_angle_attrs = {}
polar_angle_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree'])
NXdetector_elems['polar_angle'] = NXelem_type(name='polar_angle', types = [5],  dims = [-1], info = 'Polar angle of detector element', desc = '', minOccur = 0, enums = [],  attrs = polar_angle_attrs)

hold_off_attrs = {}
hold_off_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['micro.second'])
NXdetector_elems['hold_off'] = NXelem_type(name='hold_off', types = [5],  dims = [-1], info = 'Delay in detector registering an event', desc = '', minOccur = 0, enums = [],  attrs = hold_off_attrs)

translation_attrs = {}
translation_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['centimeter'])
NXdetector_elems['translation'] = NXelem_type(name='translation', types = [5],  dims = [-1], info = 'translation normal to direct beam', desc = '', minOccur = 0, enums = [],  attrs = translation_attrs)

calibration_date_attrs = {}
NXdetector_elems['calibration_date'] = NXelem_type(name='calibration_date', types = [4],  dims = [-1], info = 'date of last calibration (geometry and/or efficiency)  measurements', desc = '', minOccur = 0, enums = [],  attrs = calibration_date_attrs)

type_attrs = {}
NXdetector_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"He3 gas cylinder"', 'He3 PSD"', '"He3 planar multidetector"', '"He3 curved multidetector"', '"multi-tube He3 PSD"', '"BF3 gas"', '"scintillator"', '"fission chamber"'],  attrs = type_attrs)

id_attrs = {}
NXdetector_elems['id'] = NXelem_type(name='id', types = [24],  dims = [-1], info = 'Identifier of detector element', desc = '', minOccur = 0, enums = [],  attrs = id_attrs)

pixel_size_attrs = {}
pixel_size_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['millimetre'])
NXdetector_elems['pixel_size'] = NXelem_type(name='pixel_size', types = [5],  dims = [-1], info = 'Size of the pixel (1D or 2D)', desc = '', minOccur = 0, enums = [],  attrs = pixel_size_attrs)

gas_pressure_attrs = {}
gas_pressure_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['bars'])
NXdetector_elems['gas_pressure'] = NXelem_type(name='gas_pressure', types = [5],  dims = [-1], info = 'Detector gas pressure', desc = '', minOccur = 0, enums = [],  attrs = gas_pressure_attrs)
NXdetector_groups = {}
NXdetector_groups['NXnote'] = NXgroup_type(nxclass='NXnote', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXdetector_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXdetector_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXdetector'] = NXgroup_type(nxclass = 'NXdetector', minOccur= 0, maxOccur= 0, attrs= NXdetector_attrs, elems= NXdetector_elems, groups= NXdetector_groups)


NXdisc_chopper_attrs = {}
NXdisc_chopper_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['chopper_name'])

NXdisc_chopper_elems = {}

slit_angle_attrs = {}
slit_angle_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree'])
NXdisc_chopper_elems['slit_angle'] = NXelem_type(name='slit_angle', types = [5],  dims = [1], info = 'angular opening', desc = '', minOccur = 1, enums = [],  attrs = slit_angle_attrs)

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXdisc_chopper_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'distance of chopper to previous item', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = distance_attrs)

slit_height_attrs = {}
slit_height_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXdisc_chopper_elems['slit_height'] = NXelem_type(name='slit_height', types = [5],  dims = [1], info = 'total slit height', desc = '', minOccur = 1, enums = ['  '],  attrs = slit_height_attrs)

radius_attrs = {}
radius_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXdisc_chopper_elems['radius'] = NXelem_type(name='radius', types = [5],  dims = [1], info = 'radius to centre of slit', desc = '', minOccur = 1, enums = [' '],  attrs = radius_attrs)

type_attrs = {}
NXdisc_chopper_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Chopper type single|contra_rotating_pair|synchro_pair', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

phase_attrs = {}
phase_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree'])
NXdisc_chopper_elems['phase'] = NXelem_type(name='phase', types = [5],  dims = [1], info = 'chopper phase angle', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = phase_attrs)

pair_separation_attrs = {}
pair_separation_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXdisc_chopper_elems['pair_separation'] = NXelem_type(name='pair_separation', types = [5],  dims = [1], info = 'disc spacing in direction of beam', desc = '', minOccur = 0, enums = [' '],  attrs = pair_separation_attrs)

rotation_speed_attrs = {}
rotation_speed_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['rpm'])
NXdisc_chopper_elems['rotation_speed'] = NXelem_type(name='rotation_speed', types = [5],  dims = [1], info = 'chopper rotation speed', desc = '', minOccur = 1, enums = [' \n      ?\n   '],  attrs = rotation_speed_attrs)

ratio_attrs = {}
NXdisc_chopper_elems['ratio'] = NXelem_type(name='ratio', types = [24],  dims = [1], info = 'pulse reduction factor of this chopper in relation to other choppers/fastest pulse      in the instrument', desc = '', minOccur = 1, enums = ['\n     {pulse reduction factor of this chopper in relation to other choppers/fastest pulse\n     in the instrument}?\n   '],  attrs = ratio_attrs)

slits_attrs = {}
NXdisc_chopper_elems['slits'] = NXelem_type(name='slits', types = [24],  dims = [1], info = 'Number of slits', desc = '', minOccur = 1, enums = [],  attrs = slits_attrs)
NXdisc_chopper_groups = {}

group_types['NXdisc_chopper'] = NXgroup_type(nxclass = 'NXdisc_chopper', minOccur= 0, maxOccur= 0, attrs= NXdisc_chopper_attrs, elems= NXdisc_chopper_elems, groups= NXdisc_chopper_groups)


NXentry_attrs = {}
NXentry_attrs['name'] = NXattr_type(name='name', info = 'Entry Name', desc = '', minOccur = 1, enums = [])

NXentry_elems = {}

definition_attrs = {}
definition_attrs['URL'] = NXattr_type(name='URL', info = 'URL of DTD file', desc = '', minOccur = 1, enums = [])
definition_attrs['version'] = NXattr_type(name='version', info = 'DTD version number', desc = '', minOccur = 1, enums = [])
NXentry_elems['definition'] = NXelem_type(name='definition', types = [4],  dims = [-1], info = 'Name of entry DTD', desc = '', minOccur = 0, enums = [],  attrs = definition_attrs)

run_number_attrs = {}
NXentry_elems['run_number'] = NXelem_type(name='run_number', types = [24],  dims = [1], info = 'Number of run or scan stored in this entry', desc = '', minOccur = 0, enums = [],  attrs = run_number_attrs)

title_attrs = {}
NXentry_elems['title'] = NXelem_type(name='title', types = [4],  dims = [-1], info = 'Extended title for entry', desc = '', minOccur = 0, enums = [],  attrs = title_attrs)

run_cycle_attrs = {}
NXentry_elems['run_cycle'] = NXelem_type(name='run_cycle', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = [],  attrs = run_cycle_attrs)

notes_attrs = {}
NXentry_elems['notes'] = NXelem_type(name='notes', types = [4],  dims = [-1], info = 'Notes describing entry', desc = '', minOccur = 0, enums = [],  attrs = notes_attrs)

command_line_attrs = {}
NXentry_elems['command_line'] = NXelem_type(name='command_line', types = [4],  dims = [-1], info = 'Name of command line used to generate this file', desc = '', minOccur = 0, enums = [],  attrs = command_line_attrs)

experiment_identifier_attrs = {}
NXentry_elems['experiment_identifier'] = NXelem_type(name='experiment_identifier', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = [],  attrs = experiment_identifier_attrs)

end_time_attrs = {}
NXentry_elems['end_time'] = NXelem_type(name='end_time', types = [4],  dims = [-1], info = 'Ending time of measurement', desc = '', minOccur = 0, enums = [],  attrs = end_time_attrs)

duration_attrs = {}
duration_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['seconds'])
NXentry_elems['duration'] = NXelem_type(name='duration', types = [24],  dims = [1], info = 'Duration of measurement', desc = '', minOccur = 0, enums = [],  attrs = duration_attrs)

program_name_attrs = {}
program_name_attrs['version'] = NXattr_type(name='version', info = 'Program version number', desc = '', minOccur = 1, enums = [])
NXentry_elems['program_name'] = NXelem_type(name='program_name', types = [4],  dims = [-1], info = 'Name of program used to generate this file', desc = '', minOccur = 0, enums = [],  attrs = program_name_attrs)

start_time_attrs = {}
NXentry_elems['start_time'] = NXelem_type(name='start_time', types = [4],  dims = [-1], info = 'Starting time of measurement', desc = '', minOccur = 0, enums = [],  attrs = start_time_attrs)
NXentry_groups = {}
NXentry_groups['NXinstrument'] = NXgroup_type(nxclass='NXinstrument', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXentry_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXentry_groups['NXsample'] = NXgroup_type(nxclass='NXsample', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXentry_groups['NXuser'] = NXgroup_type(nxclass='NXuser', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXentry_groups['NXmonitor'] = NXgroup_type(nxclass='NXmonitor', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXentry'] = NXgroup_type(nxclass = 'NXentry', minOccur= 0, maxOccur= 0, attrs= NXentry_attrs, elems= NXentry_elems, groups= NXentry_groups)


NXenvironment_attrs = {}
NXenvironment_attrs['name'] = NXattr_type(name='name', info = 'Name of sample environment', desc = '', minOccur = 1, enums = [])

NXenvironment_elems = {}

program_attrs = {}
NXenvironment_elems['program'] = NXelem_type(name='program', types = [4],  dims = [-1], info = 'Program controlling the apparatus; e.g. LabView VI name', desc = '', minOccur = 0, enums = [],  attrs = program_attrs)

type_attrs = {}
NXenvironment_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Type of apparatus.  This could be the SE codes in scheduling database; e.g. OC/100', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

name_attrs = {}
NXenvironment_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Apparatus identification code/model number; e.g.  OC100 011 ', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)

short_name_attrs = {}
NXenvironment_elems['short_name'] = NXelem_type(name='short_name', types = [4],  dims = [-1], info = 'Alternative short name, perhaps for dashboard display like a present Seblock name', desc = '', minOccur = 0, enums = [],  attrs = short_name_attrs)

description_attrs = {}
NXenvironment_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'Description of the apparatus; e.g.  100mm bore orange cryostat with Roots pump ', desc = '', minOccur = 0, enums = [],  attrs = description_attrs)
NXenvironment_groups = {}
NXenvironment_groups['NXnote'] = NXgroup_type(nxclass='NXnote', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXenvironment_groups['NXsensor'] = NXgroup_type(nxclass='NXsensor', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXenvironment_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXenvironment'] = NXgroup_type(nxclass = 'NXenvironment', minOccur= 0, maxOccur= 0, attrs= NXenvironment_attrs, elems= NXenvironment_elems, groups= NXenvironment_groups)


NXevent_data_attrs = {}
NXevent_data_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = [])

NXevent_data_elems = {}
NXevent_data_groups = {}

group_types['NXevent_data'] = NXgroup_type(nxclass = 'NXevent_data', minOccur= 0, maxOccur= 0, attrs= NXevent_data_attrs, elems= NXevent_data_elems, groups= NXevent_data_groups)


NXfermi_chopper_attrs = {}
NXfermi_chopper_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['chopper_name'])

NXfermi_chopper_elems = {}

width_attrs = {}
width_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfermi_chopper_elems['width'] = NXelem_type(name='width', types = [5],  dims = [1], info = 'input beam width', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = width_attrs)

slit_attrs = {}
slit_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfermi_chopper_elems['slit'] = NXelem_type(name='slit', types = [5],  dims = [1], info = 'width of an individual slit', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = slit_attrs)

num_attrs = {}
NXfermi_chopper_elems['num'] = NXelem_type(name='num', types = [24],  dims = [1], info = 'number of slits', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = num_attrs)

radius_attrs = {}
radius_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfermi_chopper_elems['radius'] = NXelem_type(name='radius', types = [5],  dims = [1], info = 'radius of chopper', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = radius_attrs)

type_attrs = {}
NXfermi_chopper_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'fchopper type', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

rotation_speed_attrs = {}
rotation_speed_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['rpm'])
NXfermi_chopper_elems['rotation_speed'] = NXelem_type(name='rotation_speed', types = [5],  dims = [1], info = 'chopper rotation speed', desc = '', minOccur = 1, enums = [' \n      ?\n   '],  attrs = rotation_speed_attrs)

r_slit_attrs = {}
r_slit_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfermi_chopper_elems['r_slit'] = NXelem_type(name='r_slit', types = [5],  dims = [1], info = 'radius of curvature of slits', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = r_slit_attrs)

height_attrs = {}
height_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfermi_chopper_elems['height'] = NXelem_type(name='height', types = [5],  dims = [1], info = 'input beam height', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = height_attrs)
NXfermi_chopper_groups = {}

group_types['NXfermi_chopper'] = NXgroup_type(nxclass = 'NXfermi_chopper', minOccur= 0, maxOccur= 0, attrs= NXfermi_chopper_attrs, elems= NXfermi_chopper_elems, groups= NXfermi_chopper_groups)


NXfilter_attrs = {}
NXfilter_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['some_filter'])

NXfilter_elems = {}

status_attrs = {}
NXfilter_elems['status'] = NXelem_type(name='status', types = [4],  dims = [-1], info = 'in | out', desc = '', minOccur = 1, enums = [' ?\n  '],  attrs = status_attrs)

position_attrs = {}
position_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXfilter_elems['position'] = NXelem_type(name='position', types = [5],  dims = [-1], info = 'position of the element in relation to whatever coordinate      system we agree upon', desc = '', minOccur = 1, enums = ['\n    {position of the element in relation to whatever coordinate\n     system we agree upon}?\n  '],  attrs = position_attrs)

type_attrs = {}
NXfilter_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Description of filter', desc = '', minOccur = 1, enums = ['\n    ?\n  '],  attrs = type_attrs)
NXfilter_groups = {}

group_types['NXfilter'] = NXgroup_type(nxclass = 'NXfilter', minOccur= 0, maxOccur= 0, attrs= NXfilter_attrs, elems= NXfilter_elems, groups= NXfilter_groups)


NXflipper_attrs = {}
NXflipper_attrs['name'] = NXattr_type(name='name', info = 'Name of flipper', desc = '', minOccur = 1, enums = [])

NXflipper_elems = {}

guide_turns_attrs = {}
NXflipper_elems['guide_turns'] = NXelem_type(name='guide_turns', types = [5],  dims = [1], info = 'Number of turns/cm in guide field coils', desc = '', minOccur = 0, enums = [],  attrs = guide_turns_attrs)

comp_current_attrs = {}
comp_current_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['amperes'])
NXflipper_elems['comp_current'] = NXelem_type(name='comp_current', types = [5],  dims = [1], info = 'Compensating field coil current in "on" state"', desc = '', minOccur = 0, enums = [],  attrs = comp_current_attrs)

guide_current_attrs = {}
guide_current_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['amperes'])
NXflipper_elems['guide_current'] = NXelem_type(name='guide_current', types = [5],  dims = [1], info = 'Guide field coil current in "on" state"', desc = '', minOccur = 0, enums = [],  attrs = guide_current_attrs)

flip_turns_attrs = {}
NXflipper_elems['flip_turns'] = NXelem_type(name='flip_turns', types = [5],  dims = [1], info = 'Number of turns/cm in flipping field coils', desc = '', minOccur = 0, enums = [],  attrs = flip_turns_attrs)

thickness_attrs = {}
thickness_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXflipper_elems['thickness'] = NXelem_type(name='thickness', types = [5],  dims = [1], info = 'thickness along path of neutron travel', desc = '', minOccur = 0, enums = [],  attrs = thickness_attrs)

type_attrs = {}
NXflipper_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'coil|current-sheet', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

comp_turns_attrs = {}
NXflipper_elems['comp_turns'] = NXelem_type(name='comp_turns', types = [5],  dims = [1], info = 'Number of turns/cm in compensating field coils', desc = '', minOccur = 0, enums = [],  attrs = comp_turns_attrs)

flip_current_attrs = {}
flip_current_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['amperes'])
NXflipper_elems['flip_current'] = NXelem_type(name='flip_current', types = [5],  dims = [1], info = 'Flipping field coil current in "on" state"', desc = '', minOccur = 0, enums = [],  attrs = flip_current_attrs)
NXflipper_groups = {}

group_types['NXflipper'] = NXgroup_type(nxclass = 'NXflipper', minOccur= 0, maxOccur= 0, attrs= NXflipper_attrs, elems= NXflipper_elems, groups= NXflipper_groups)


NXgeometry_attrs = {}
NXgeometry_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['geometry'])

NXgeometry_elems = {}

component_index_attrs = {}
NXgeometry_elems['component_index'] = NXelem_type(name='component_index', types = [24],  dims = [1], info = 'Position of the component along the beam path.', desc = 'The sample is at 0, components upstream have negative component_index, components downstream have positive component_index.', minOccur = 0, enums = [],  attrs = component_index_attrs)

description_attrs = {}
NXgeometry_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'Optional description/label', desc = 'Probably only present if we are an additional reference point for components rather than the location of a real component', minOccur = 0, enums = [],  attrs = description_attrs)
NXgeometry_groups = {}
NXgeometry_groups['NXtranslation'] = NXgroup_type(nxclass='NXtranslation', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXgeometry_groups['NXshape'] = NXgroup_type(nxclass='NXshape', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXgeometry_groups['NXorientation'] = NXgroup_type(nxclass='NXorientation', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXgeometry'] = NXgroup_type(nxclass = 'NXgeometry', minOccur= 0, maxOccur= 0, attrs= NXgeometry_attrs, elems= NXgeometry_elems, groups= NXgeometry_groups)


NXguide_attrs = {}
NXguide_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = [])

NXguide_elems = {}
NXguide_groups = {}

group_types['NXguide'] = NXgroup_type(nxclass = 'NXguide', minOccur= 0, maxOccur= 0, attrs= NXguide_attrs, elems= NXguide_elems, groups= NXguide_groups)


NXinstrument_attrs = {}
NXinstrument_attrs['name'] = NXattr_type(name='name', info = 'Name of instrument', desc = '', minOccur = 1, enums = [])

NXinstrument_elems = {}

name_attrs = {}
name_attrs['short_name'] = NXattr_type(name='short_name', info = 'abbreviated name of instrument', desc = '', minOccur = 1, enums = [])
NXinstrument_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Name of instrument', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)
NXinstrument_groups = {}
NXinstrument_groups['NXsource'] = NXgroup_type(nxclass='NXsource', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXdetector'] = NXgroup_type(nxclass='NXdetector', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXattenuator'] = NXgroup_type(nxclass='NXattenuator', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXfermi_chopper'] = NXgroup_type(nxclass='NXfermi_chopper', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXflipper'] = NXgroup_type(nxclass='NXflipper', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXcrystal'] = NXgroup_type(nxclass='NXcrystal', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXfilter'] = NXgroup_type(nxclass='NXfilter', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXvelocity_selector'] = NXgroup_type(nxclass='NXvelocity_selector', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXdisk_chopper'] = NXgroup_type(nxclass='NXdisk_chopper', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXmirror'] = NXgroup_type(nxclass='NXmirror', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXcollimator'] = NXgroup_type(nxclass='NXcollimator', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXaperture'] = NXgroup_type(nxclass='NXaperture', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXpolarizer'] = NXgroup_type(nxclass='NXpolarizer', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXguide'] = NXgroup_type(nxclass='NXguide', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXinstrument_groups['NXbeam_stop'] = NXgroup_type(nxclass='NXbeam_stop', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXinstrument'] = NXgroup_type(nxclass = 'NXinstrument', minOccur= 0, maxOccur= 0, attrs= NXinstrument_attrs, elems= NXinstrument_elems, groups= NXinstrument_groups)


NXlog_attrs = {}
NXlog_attrs['name'] = NXattr_type(name='name', info = 'Name of logged measurements', desc = '', minOccur = 1, enums = [])

NXlog_elems = {}

raw_value_attrs = {}
raw_value_attrs['units'] = NXattr_type(name='units', info = 'units of raw values', desc = '', minOccur = 1, enums = [])
NXlog_elems['raw_value'] = NXelem_type(name='raw_value', types = [5, 24],  dims = [1], info = 'Array of raw information, such as voltage on a thermocouple', desc = '', minOccur = 0, enums = [],  attrs = raw_value_attrs)

description_attrs = {}
NXlog_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'Description of logged value', desc = '', minOccur = 0, enums = [],  attrs = description_attrs)

value_attrs = {}
value_attrs['units'] = NXattr_type(name='units', info = 'units of logged value', desc = '', minOccur = 1, enums = [])
NXlog_elems['value'] = NXelem_type(name='value', types = [5, 24],  dims = [1], info = 'Array of logged value, such as temperature', desc = '', minOccur = 1, enums = [],  attrs = value_attrs)

time_attrs = {}
time_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
time_attrs['start'] = NXattr_type(name='start', info = 'ISO8601', desc = '', minOccur = 1, enums = [])
NXlog_elems['time'] = NXelem_type(name='time', types = [5],  dims = [1], info = 'Time of logged entry', desc = '', minOccur = 1, enums = [],  attrs = time_attrs)
NXlog_groups = {}

group_types['NXlog'] = NXgroup_type(nxclass = 'NXlog', minOccur= 0, maxOccur= 0, attrs= NXlog_attrs, elems= NXlog_elems, groups= NXlog_groups)


NXmirror_attrs = {}
NXmirror_attrs['name'] = NXattr_type(name='name', info = 'Name of mirror guide', desc = '', minOccur = 1, enums = [])

NXmirror_elems = {}
NXmirror_groups = {}

group_types['NXmirror'] = NXgroup_type(nxclass = 'NXmirror', minOccur= 0, maxOccur= 0, attrs= NXmirror_attrs, elems= NXmirror_elems, groups= NXmirror_groups)


NXmoderator_attrs = {}
NXmoderator_attrs['name'] = NXattr_type(name='name', info = 'Name of moderator', desc = '', minOccur = 1, enums = [])

NXmoderator_elems = {}

distance_attrs = {}
NXmoderator_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Effective distance as seen by measuring radiation', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

poison_depth_attrs = {}
poison_depth_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXmoderator_elems['poison_depth'] = NXelem_type(name='poison_depth', types = [5],  dims = [1], info = 'Poison depth', desc = '', minOccur = 0, enums = [],  attrs = poison_depth_attrs)

temperature_attrs = {}
temperature_attrs['Units'] = NXattr_type(name='Units', info = '', desc = '', minOccur = 1, enums = ['Kelvin'])
NXmoderator_elems['temperature'] = NXelem_type(name='temperature', types = [5],  dims = [1], info = 'average/nominal moderator temperature', desc = '', minOccur = 1, enums = [],  attrs = temperature_attrs)

coupled_attrs = {}
NXmoderator_elems['coupled'] = NXelem_type(name='coupled', types = [21],  dims = [1], info = 'whether the moderator is coupled', desc = '', minOccur = 0, enums = [],  attrs = coupled_attrs)

type_attrs = {}
NXmoderator_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = ' "H20" | "D20"  |  "Liquid H2"  | "Liquid CH4" | "Liquid D2" | "Solid D2" | "C" |"Solid CH4" | "Solid H2"', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

poison_material_attrs = {}
NXmoderator_elems['poison_material'] = NXelem_type(name='poison_material', types = [4],  dims = [-1], info = ' Gd | Cd |...', desc = '', minOccur = 1, enums = [],  attrs = poison_material_attrs)
NXmoderator_groups = {}
NXmoderator_groups['NXlog'] = NXgroup_type(nxclass='NXlog', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXmoderator_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXmoderator_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXmoderator'] = NXgroup_type(nxclass = 'NXmoderator', minOccur= 0, maxOccur= 0, attrs= NXmoderator_attrs, elems= NXmoderator_elems, groups= NXmoderator_groups)


NXmonitor_attrs = {}
NXmonitor_attrs['name'] = NXattr_type(name='name', info = 'Name of monitor', desc = '', minOccur = 1, enums = [])

NXmonitor_elems = {}

time_of_flight_attrs = {}
time_of_flight_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['microseconds'])
NXmonitor_elems['time_of_flight'] = NXelem_type(name='time_of_flight', types = [5],  dims = [-1], info = 'Time-of-flight', desc = '', minOccur = 0, enums = [],  attrs = time_of_flight_attrs)

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXmonitor_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance of monitor from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

range_attrs = {}
NXmonitor_elems['range'] = NXelem_type(name='range', types = [5],  dims = [-1], info = 'Time-of-flight range over which the integral was calculated', desc = '', minOccur = 0, enums = [],  attrs = range_attrs)

preset_attrs = {}
NXmonitor_elems['preset'] = NXelem_type(name='preset', types = [5],  dims = [1], info = 'preset value for time or monitor', desc = '', minOccur = 0, enums = [],  attrs = preset_attrs)

mode_attrs = {}
NXmonitor_elems['mode'] = NXelem_type(name='mode', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"monitor"', '"timer"'],  attrs = mode_attrs)

efficiency_attrs = {}
NXmonitor_elems['efficiency'] = NXelem_type(name='efficiency', types = [5],  dims = [-1], info = 'Monitor efficiency', desc = '', minOccur = 0, enums = [],  attrs = efficiency_attrs)

type_attrs = {}
NXmonitor_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"Fission Chamber"', '"Scintillator"'],  attrs = type_attrs)

sampled_fraction_attrs = {}
sampled_fraction_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['dimensionless'])
NXmonitor_elems['sampled_fraction'] = NXelem_type(name='sampled_fraction', types = [5],  dims = [1], info = 'Proportion of incident beam sampled by the monitor (0<x<1)', desc = '', minOccur = 1, enums = [],  attrs = sampled_fraction_attrs)

integral_attrs = {}
integral_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXmonitor_elems['integral'] = NXelem_type(name='integral', types = [5],  dims = [1], info = 'Total integral monitor counts', desc = '', minOccur = 0, enums = [],  attrs = integral_attrs)

data_attrs = {}
data_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
data_attrs['signal'] = NXattr_type(name='signal', info = '', desc = '', minOccur = 1, enums = ['1'])
data_attrs['axes'] = NXattr_type(name='axes', info = '', desc = '', minOccur = 1, enums = [])
NXmonitor_elems['data'] = NXelem_type(name='data', types = [24],  dims = [-1], info = 'Monitor data', desc = '', minOccur = 0, enums = [],  attrs = data_attrs)
NXmonitor_groups = {}
NXmonitor_groups['NXlog'] = NXgroup_type(nxclass='NXlog', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXmonitor_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXmonitor'] = NXgroup_type(nxclass = 'NXmonitor', minOccur= 0, maxOccur= 0, attrs= NXmonitor_attrs, elems= NXmonitor_elems, groups= NXmonitor_groups)


NXnote_attrs = {}
NXnote_attrs['name'] = NXattr_type(name='name', info = 'name of note', desc = '', minOccur = 1, enums = [])

NXnote_elems = {}

description_attrs = {}
NXnote_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'Title of an image or other details of the note', desc = '', minOccur = 0, enums = [],  attrs = description_attrs)

author_attrs = {}
NXnote_elems['author'] = NXelem_type(name='author', types = [4],  dims = [-1], info = 'Author or creator of note', desc = '', minOccur = 0, enums = [],  attrs = author_attrs)

date_attrs = {}
NXnote_elems['date'] = NXelem_type(name='date', types = [4],  dims = [-1], info = 'Date note created/added', desc = '', minOccur = 0, enums = [],  attrs = date_attrs)

file_name_attrs = {}
NXnote_elems['file_name'] = NXelem_type(name='file_name', types = [4],  dims = [-1], info = 'Name of original file name if note was read from an external source', desc = '', minOccur = 0, enums = [],  attrs = file_name_attrs)

type_attrs = {}
NXnote_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Mime content type of note data field e.g. image/jpeg, text/plain, text/html', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

data_attrs = {}
NXnote_elems['data'] = NXelem_type(name='data', types = [4],  dims = [-1], info = 'Binary note data - if text, line terminator is \r\n.', desc = '', minOccur = 0, enums = [],  attrs = data_attrs)
NXnote_groups = {}

group_types['NXnote'] = NXgroup_type(nxclass = 'NXnote', minOccur= 0, maxOccur= 0, attrs= NXnote_attrs, elems= NXnote_elems, groups= NXnote_groups)


NXorientation_attrs = {}
NXorientation_attrs['name'] = NXattr_type(name='name', info = 'name of orientation', desc = '', minOccur = 1, enums = [])

NXorientation_elems = {}

value_attrs = {}
NXorientation_elems['value'] = NXelem_type(name='value', types = [5],  dims = ['numobj', 6], info = 'The orientation information is stored as direction cosines.', desc = 'The direction cosines will be between the local coordinate directions and the reference directions (to origin or relative NXgeometry). Calling the local unit vectors (x\',y\',z\') and the reference unit vectors (x,y,z) the six numbers will be [x\' dot x, x\' dot y, x\' dot z, y\' dot x, y\' dot y, y\' dot z] where "dot" is the scalar dot product (cosine of the angle between the unit vectors). The unit vectors in both the local and reference coordinates are right-handed and orthonormal.', minOccur = 0, enums = ['{The direction cosines will be between the local coordinate directions and the reference directions (to origin or relative NXgeometry). Calling the local unit vectors (x\',y\',z\') and the reference unit vectors (x,y,z) the six numbers will be [x\' dot x, x\' dot y, x\' dot z, y\' dot x, y\' dot y, y\' dot z] where "dot" is the scalar dot product (cosine of the angle between the unit vectors). The unit vectors in both the local and reference coordinates are right-handed and orthonormal.}'],  attrs = value_attrs)
NXorientation_groups = {}
NXorientation_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXorientation'] = NXgroup_type(nxclass = 'NXorientation', minOccur= 0, maxOccur= 0, attrs= NXorientation_attrs, elems= NXorientation_elems, groups= NXorientation_groups)


NXpolarizer_attrs = {}
NXpolarizer_attrs['name'] = NXattr_type(name='name', info = 'Name of polarizer', desc = '', minOccur = 1, enums = [])

NXpolarizer_elems = {}

efficiency_attrs = {}
NXpolarizer_elems['efficiency'] = NXelem_type(name='efficiency', types = [4],  dims = [-1], info = 'polarizing efficiency', desc = '', minOccur = 0, enums = [],  attrs = efficiency_attrs)

type_attrs = {}
NXpolarizer_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'crystal,supermirror', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)

composition_attrs = {}
NXpolarizer_elems['composition'] = NXelem_type(name='composition', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = [],  attrs = composition_attrs)

reflection_attrs = {}
NXpolarizer_elems['reflection'] = NXelem_type(name='reflection', types = [24],  dims = [-1], info = '[hkl] values of nominal reflection', desc = '', minOccur = 1, enums = [' ? '],  attrs = reflection_attrs)
NXpolarizer_groups = {}

group_types['NXpolarizer'] = NXgroup_type(nxclass = 'NXpolarizer', minOccur= 0, maxOccur= 0, attrs= NXpolarizer_attrs, elems= NXpolarizer_elems, groups= NXpolarizer_groups)


NXroot_attrs = {}
NXroot_attrs['NeXus_version'] = NXattr_type(name='NeXus_version', info = 'Version of NeXus API used in writing the file', desc = '', minOccur = 1, enums = [])
NXroot_attrs['HDF5_version'] = NXattr_type(name='HDF5_version', info = '', desc = '', minOccur = 0, enums = [])
NXroot_attrs['creator'] = NXattr_type(name='creator', info = 'facility or program where file originated', desc = '', minOccur = 0, enums = [])
NXroot_attrs['file_name'] = NXattr_type(name='file_name', info = 'File name of original NeXus file', desc = '', minOccur = 1, enums = [])
NXroot_attrs['file_update_time'] = NXattr_type(name='file_update_time', info = 'Date and time of last file change at close', desc = '', minOccur = 1, enums = [])
NXroot_attrs['HDF_version'] = NXattr_type(name='HDF_version', info = '', desc = '', minOccur = 0, enums = [])
NXroot_attrs['file_time'] = NXattr_type(name='file_time', info = 'Date and time of file creation', desc = '', minOccur = 1, enums = [])

NXroot_elems = {}
NXroot_groups = {}
NXroot_groups['NXentry'] = NXgroup_type(nxclass='NXentry', minOccur = 1,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXroot'] = NXgroup_type(nxclass = 'NXroot', minOccur= 0, maxOccur= 0, attrs= NXroot_attrs, elems= NXroot_elems, groups= NXroot_groups)


NXsample_attrs = {}
NXsample_attrs['name'] = NXattr_type(name='name', info = 'name of the sample', desc = '', minOccur = 1, enums = [])

NXsample_elems = {}

changer_position_attrs = {}
NXsample_elems['changer_position'] = NXelem_type(name='changer_position', types = [24],  dims = [1], info = 'Sample changer position', desc = '', minOccur = 0, enums = [],  attrs = changer_position_attrs)

description_attrs = {}
NXsample_elems['description'] = NXelem_type(name='description', types = [4],  dims = [-1], info = 'Description of the sample', desc = '', minOccur = 0, enums = [],  attrs = description_attrs)

sample_component_attrs = {}
NXsample_elems['sample_component'] = NXelem_type(name='sample_component', types = [4],  dims = [-1], info = ' What type of component we are "sample  |  can  |  atmosphere  |  kit" ', desc = '', minOccur = 0, enums = [],  attrs = sample_component_attrs)

magnetic_field_attrs = {}
magnetic_field_attrs['direction'] = NXattr_type(name='direction', info = '', desc = '', minOccur = 1, enums = ['x', 'y', 'z'])
NXsample_elems['magnetic_field'] = NXelem_type(name='magnetic_field', types = [5],  dims = [-1], info = 'Applied magnetic field', desc = '', minOccur = 0, enums = [],  attrs = magnetic_field_attrs)

unit_cell_class_attrs = {}
NXsample_elems['unit_cell_class'] = NXelem_type(name='unit_cell_class', types = [4],  dims = [-1], info = ' In case it is all we know and we want to record it "cubic  |  tetragonal  |  orthorhombic  |  monoclinic  |  triclinic" ', desc = '', minOccur = 0, enums = [],  attrs = unit_cell_class_attrs)

component_attrs = {}
NXsample_elems['component'] = NXelem_type(name='component', types = [4],  dims = [-1], info = 'Details of the component of the sample and/or can', desc = '', minOccur = 0, enums = [],  attrs = component_attrs)

unit_cell_group_attrs = {}
NXsample_elems['unit_cell_group'] = NXelem_type(name='unit_cell_group', types = [4],  dims = [-1], info = 'Crystallographic point or space group', desc = '', minOccur = 0, enums = [],  attrs = unit_cell_group_attrs)

pressure_attrs = {}
NXsample_elems['pressure'] = NXelem_type(name='pressure', types = [5],  dims = [-1], info = 'Applied pressure', desc = '', minOccur = 0, enums = [],  attrs = pressure_attrs)

electric_field_attrs = {}
electric_field_attrs['direction'] = NXattr_type(name='direction', info = '', desc = '', minOccur = 1, enums = ['x', 'y', 'z'])
NXsample_elems['electric_field'] = NXelem_type(name='electric_field', types = [5],  dims = [-1], info = 'Applied electric field', desc = '', minOccur = 0, enums = [],  attrs = electric_field_attrs)

orientation_matrix_attrs = {}
NXsample_elems['orientation_matrix'] = NXelem_type(name='orientation_matrix', types = [5],  dims = ['n_comp', 3, 3], info = 'Orientation matrix of single crystal sample', desc = 'The is the orientation matrix using Busing-Levy convention', minOccur = 0, enums = [],  attrs = orientation_matrix_attrs)

concentration_attrs = {}
concentration_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['g.cm-3'])
NXsample_elems['concentration'] = NXelem_type(name='concentration', types = [5],  dims = [-1], info = 'Concentration of each component', desc = '', minOccur = 0, enums = [],  attrs = concentration_attrs)

unit_cell_attrs = {}
NXsample_elems['unit_cell'] = NXelem_type(name='unit_cell', types = [5],  dims = ['n_comp', 6], info = 'Unit cell parameters (lengths and angles)', desc = '', minOccur = 0, enums = [],  attrs = unit_cell_attrs)

mass_attrs = {}
mass_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['g'])
NXsample_elems['mass'] = NXelem_type(name='mass', types = [5],  dims = [-1], info = 'Mass of sample', desc = '', minOccur = 0, enums = [],  attrs = mass_attrs)

temperature_attrs = {}
NXsample_elems['temperature'] = NXelem_type(name='temperature', types = [5],  dims = [-1], info = 'Sample temperature. This could be a scanned variable', desc = '', minOccur = 0, enums = [],  attrs = temperature_attrs)

name_attrs = {}
NXsample_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Descriptive name of sample', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)

unit_cell_volume_attrs = {}
unit_cell_volume_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Angstroms3'])
unit_cell_volume_attrs['rank'] = NXattr_type(name='rank', info = '', desc = '', minOccur = 1, enums = ['1'])
NXsample_elems['unit_cell_volume'] = NXelem_type(name='unit_cell_volume', types = [5],  dims = [-1], info = 'Volume of the unit cell', desc = '', minOccur = 0, enums = [],  attrs = unit_cell_volume_attrs)

density_attrs = {}
density_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['g cm-3'])
NXsample_elems['density'] = NXelem_type(name='density', types = [5],  dims = [-1], info = 'Density of sample', desc = '', minOccur = 0, enums = [],  attrs = density_attrs)

path_length_window_attrs = {}
NXsample_elems['path_length_window'] = NXelem_type(name='path_length_window', types = [5],  dims = [1], info = 'Thickness of a beam entry/exit window on the can (mm) - assumed same for entry and exit', desc = '', minOccur = 0, enums = [],  attrs = path_length_window_attrs)

preparation_date_attrs = {}
NXsample_elems['preparation_date'] = NXelem_type(name='preparation_date', types = [4],  dims = [-1], info = 'Date of preparation of the sample', desc = '', minOccur = 0, enums = [],  attrs = preparation_date_attrs)

path_length_attrs = {}
NXsample_elems['path_length'] = NXelem_type(name='path_length', types = [5],  dims = [1], info = 'Path length through sample/can for simple case when it does not vary with scattering direction', desc = '', minOccur = 0, enums = [],  attrs = path_length_attrs)

short_title_attrs = {}
NXsample_elems['short_title'] = NXelem_type(name='short_title', types = [4],  dims = [-1], info = '20 character fixed length sample description for legends', desc = '', minOccur = 0, enums = [],  attrs = short_title_attrs)

stress_field_attrs = {}
stress_field_attrs['direction'] = NXattr_type(name='direction', info = '', desc = '', minOccur = 1, enums = ['x', 'y', 'z'])
NXsample_elems['stress_field'] = NXelem_type(name='stress_field', types = [5],  dims = [-1], info = 'External stress', desc = '', minOccur = 0, enums = [],  attrs = stress_field_attrs)

external_DAC_attrs = {}
NXsample_elems['external_DAC'] = NXelem_type(name='external_DAC', types = [5],  dims = [1], info = 'value sent to user\'s sample setup', desc = '', minOccur = 0, enums = ["{value sent to user's sample setup}"],  attrs = external_DAC_attrs)

scattering_length_density_attrs = {}
NXsample_elems['scattering_length_density'] = NXelem_type(name='scattering_length_density', types = [5],  dims = [-1], info = 'Scattering length density of each component (cm-2)', desc = '', minOccur = 0, enums = [],  attrs = scattering_length_density_attrs)

volume_fraction_attrs = {}
NXsample_elems['volume_fraction'] = NXelem_type(name='volume_fraction', types = [5],  dims = [-1], info = 'Volume fraction of each component', desc = '', minOccur = 0, enums = [],  attrs = volume_fraction_attrs)

relative_molecular_mass_attrs = {}
NXsample_elems['relative_molecular_mass'] = NXelem_type(name='relative_molecular_mass', types = [5],  dims = [-1], info = 'Relative Molecular Mass of sample', desc = '', minOccur = 0, enums = [],  attrs = relative_molecular_mass_attrs)

situation_attrs = {}
NXsample_elems['situation'] = NXelem_type(name='situation', types = [4],  dims = [-1], info = ' air  |  vacuum  |  inert atmosphere  |  oxidising atmosphere  |  reducing atmosphere  |  sealed can  |  other ', desc = 'The atmosphere will be one of the components, which is where its details will be stored; the relevant components will be indicated by the entry in the sample_component member.', minOccur = 0, enums = [],  attrs = situation_attrs)

chemical_formula_attrs = {}
NXsample_elems['chemical_formula'] = NXelem_type(name='chemical_formula', types = [4],  dims = [-1], info = 'Chemical formula', desc = '', minOccur = 0, enums = [],  attrs = chemical_formula_attrs)

type_attrs = {}
NXsample_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = ' sample  |  sample+can  |  can  |  calibration sample  |  normalisation sample  |  simulated data  |  none  |  sample environment ', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)
NXsample_groups = {}
NXsample_groups['NXlog'] = NXgroup_type(nxclass='NXlog', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXsample_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXsample_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXsample_groups['NXbeam'] = NXgroup_type(nxclass='NXbeam', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXsample_groups['NXenvironment'] = NXgroup_type(nxclass='NXenvironment', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXsample'] = NXgroup_type(nxclass = 'NXsample', minOccur= 0, maxOccur= 0, attrs= NXsample_attrs, elems= NXsample_elems, groups= NXsample_groups)


NXsensor_attrs = {}
NXsensor_attrs['name'] = NXattr_type(name='name', info = 'Name of sensor', desc = '', minOccur = 1, enums = [])

NXsensor_elems = {}

low_trip_value_attrs = {}
low_trip_value_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXsensor_elems['low_trip_value'] = NXelem_type(name='low_trip_value', types = [5],  dims = [1], info = 'Lower control bound of sensor reading if using run_control', desc = '', minOccur = 0, enums = [],  attrs = low_trip_value_attrs)

attached_to_attrs = {}
NXsensor_elems['attached_to'] = NXelem_type(name='attached_to', types = [4],  dims = [-1], info = ' where sensor is attached to ("sample"  |  "can") ', desc = '', minOccur = 0, enums = [],  attrs = attached_to_attrs)

name_attrs = {}
NXsensor_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Name for the sensor', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)

short_name_attrs = {}
NXsensor_elems['short_name'] = NXelem_type(name='short_name', types = [4],  dims = [-1], info = 'Short name of sensor used e.g. on monitor display program', desc = '', minOccur = 0, enums = [],  attrs = short_name_attrs)

run_control_attrs = {}
NXsensor_elems['run_control'] = NXelem_type(name='run_control', types = [21],  dims = [1], info = ' Is data collection controlled/synchronised to this quantity: 1=no, 0=to "value", 1=to "value_deriv1" etc.', desc = '', minOccur = 0, enums = [],  attrs = run_control_attrs)

high_trip_value_attrs = {}
high_trip_value_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXsensor_elems['high_trip_value'] = NXelem_type(name='high_trip_value', types = [5],  dims = [1], info = 'Upper control bound of sensor reading if using run_control', desc = '', minOccur = 0, enums = [],  attrs = high_trip_value_attrs)

value_attrs = {}
value_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXsensor_elems['value'] = NXelem_type(name='value', types = [5],  dims = [-1], info = 'nominal setpoint or average value - need [n] as may be a vector', desc = '', minOccur = 0, enums = [],  attrs = value_attrs)

value_deriv2_attrs = {}
value_deriv2_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXsensor_elems['value_deriv2'] = NXelem_type(name='value_deriv2', types = [5],  dims = [-1], info = 'Nominal/average second derivative of value - need [n] as may be a vector', desc = '', minOccur = 0, enums = [],  attrs = value_deriv2_attrs)

value_deriv1_attrs = {}
value_deriv1_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXsensor_elems['value_deriv1'] = NXelem_type(name='value_deriv1', types = [5],  dims = [-1], info = 'Nominal/average first derivative of value e.g. strain rate - need [n] as may be a vector', desc = '', minOccur = 0, enums = [],  attrs = value_deriv1_attrs)

measurement_attrs = {}
NXsensor_elems['measurement'] = NXelem_type(name='measurement', types = [4],  dims = [-1], info = ' what we measure "temperature  |  pH  |  magnetic_field  |  electric field  |  conductivity  |  resistance  |  voltage  |  pressure  |  flow  |  stress  |  strain  |  shear  |  surface_pressure" ', desc = '', minOccur = 0, enums = [],  attrs = measurement_attrs)

model_attrs = {}
NXsensor_elems['model'] = NXelem_type(name='model', types = [4],  dims = [-1], info = 'Sensor identification code/model number', desc = '', minOccur = 0, enums = [],  attrs = model_attrs)

type_attrs = {}
NXsensor_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = ' The type of hardware we use for the measurement e.g. Temperature:  "J  | K  | T  | E | R | S  | Pt100 | Rh/Fe" pH:  "Hg/Hg2Cl2 | Ag/AgCl |  ISFET"  Ion selective electrode: "specify species; e.g.  Ca2+"  Magnetic field:  "Hall"  Surface pressure:  "wilhelmy plate" ', desc = '', minOccur = 0, enums = ['{ The type of hardware we use for the measurement e.g. Temperature:  "J', 'K', 'T', 'E', 'R', 'S', 'Pt100', 'Rh/Fe" pH:  "Hg/Hg2Cl2', 'Ag/AgCl', 'ISFET" \nIon selective electrode: "specify species; e.g.  Ca2+" \nMagnetic field:  "Hall" \nSurface pressure:  "wilhelmy plate" }'],  attrs = type_attrs)

external_field_brief_attrs = {}
NXsensor_elems['external_field_brief'] = NXelem_type(name='external_field_brief', types = [4],  dims = [-1], info = ' along beam | across beam | transverse | solenoidal  |  flow shear gradient | flow vorticity ', desc = '', minOccur = 0, enums = [],  attrs = external_field_brief_attrs)
NXsensor_groups = {}
NXsensor_groups['NXlog'] = NXgroup_type(nxclass='NXlog', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)
NXsensor_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXsensor_groups['NXorientation'] = NXgroup_type(nxclass='NXorientation', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXsensor'] = NXgroup_type(nxclass = 'NXsensor', minOccur= 0, maxOccur= 0, attrs= NXsensor_attrs, elems= NXsensor_elems, groups= NXsensor_groups)


NXshape_attrs = {}
NXshape_attrs['name'] = NXattr_type(name='name', info = 'name of shape', desc = '', minOccur = 1, enums = [])

NXshape_elems = {}

shape_attrs = {}
NXshape_elems['shape'] = NXelem_type(name='shape', types = [4],  dims = [-1], info = '"nxcylinder", "nxbox", "nxsphere", ...', desc = '', minOccur = 0, enums = [],  attrs = shape_attrs)

size_attrs = {}
size_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['meter'])
NXshape_elems['size'] = NXelem_type(name='size', types = [5],  dims = ['numobj', 'nshapepar'], info = 'physical extent of the object along its local axes (after NXorientation) with the center of mass at the local origin (after NXtranslate).', desc = 'The meaning and location of these axes will vary according to the value of the "shape" variable. nshapepar defines how many parameters. For the "nxcylinder" type the paramters are (diameter,height). For the "nxbox" type the parameters are (length,width,height). For the "nxsphere" type the parameters are (diameter).', minOccur = 0, enums = [],  attrs = size_attrs)
NXshape_groups = {}

group_types['NXshape'] = NXgroup_type(nxclass = 'NXshape', minOccur= 0, maxOccur= 0, attrs= NXshape_attrs, elems= NXshape_elems, groups= NXshape_groups)


NXsource_attrs = {}
NXsource_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['source'])

NXsource_elems = {}

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXsource_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Effective distance from sample', desc = 'Distance as seen by radiation from sample. This number should be negative to signify that it is upstream of the sample.', minOccur = 0, enums = [],  attrs = distance_attrs)

target_material_attrs = {}
NXsource_elems['target_material'] = NXelem_type(name='target_material', types = [4],  dims = [-1], info = 'Pulsed source target material', desc = '"Ta"|"W"|"depleted_U"|"enriched_U"|"Hg"|"Pb"|"C"', minOccur = 0, enums = [' '],  attrs = target_material_attrs)

name_attrs = {}
NXsource_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Name of source', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)

power_attrs = {}
power_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['MW'])
NXsource_elems['power'] = NXelem_type(name='power', types = [5],  dims = [1], info = 'Source power', desc = '', minOccur = 0, enums = [],  attrs = power_attrs)

notes_attrs = {}
NXsource_elems['notes'] = NXelem_type(name='notes', types = [4],  dims = [-1], info = 'any source/facility related messages/events that occurred during the experiment', desc = '', minOccur = 0, enums = [],  attrs = notes_attrs)

probe_attrs = {}
NXsource_elems['probe'] = NXelem_type(name='probe', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['neutron', 'x-ray', 'muon', 'electron'],  attrs = probe_attrs)

period_attrs = {}
period_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['microseconds'])
NXsource_elems['period'] = NXelem_type(name='period', types = [5],  dims = [1], info = 'Period of pulsed source', desc = '', minOccur = 0, enums = [],  attrs = period_attrs)

current_attrs = {}
current_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['microamps'])
NXsource_elems['current'] = NXelem_type(name='current', types = [5],  dims = [1], info = 'Accelerator proton current', desc = '', minOccur = 0, enums = [],  attrs = current_attrs)

frequency_attrs = {}
frequency_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['Hz'])
NXsource_elems['frequency'] = NXelem_type(name='frequency', types = [5],  dims = [1], info = 'Frequency of pulsed source', desc = '', minOccur = 0, enums = [],  attrs = frequency_attrs)

voltage_attrs = {}
voltage_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['MeV'])
NXsource_elems['voltage'] = NXelem_type(name='voltage', types = [5],  dims = [1], info = 'Accelerator proton voltage', desc = '', minOccur = 0, enums = [],  attrs = voltage_attrs)

pulse_width_attrs = {}
pulse_width_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['micro.second'])
NXsource_elems['pulse_width'] = NXelem_type(name='pulse_width', types = [5],  dims = [1], info = 'width of source pulse', desc = '', minOccur = 0, enums = [],  attrs = pulse_width_attrs)

type_attrs = {}
NXsource_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"Spallation Neutron Source"', '"Pulsed Reactor Neutron Source"', '"Reactor Neutron Source"', '"Synchrotron X-ray Source"', '"Pulsed Muon Source"', '"Rotating Anode X-ray"', 'Fixed Tube X-ray"'],  attrs = type_attrs)
NXsource_groups = {}
NXsource_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)
NXsource_groups['NXdata'] = NXgroup_type(nxclass='NXdata', minOccur = 0,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXsource'] = NXgroup_type(nxclass = 'NXsource', minOccur= 0, maxOccur= 0, attrs= NXsource_attrs, elems= NXsource_elems, groups= NXsource_groups)


NXtranslation_attrs = {}
NXtranslation_attrs['name'] = NXattr_type(name='name', info = 'name of translation', desc = '', minOccur = 1, enums = [])

NXtranslation_elems = {}

distances_attrs = {}
distances_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = [])
NXtranslation_elems['distances'] = NXelem_type(name='distances', types = [5],  dims = ['numobj', 'numdist'], info = '(x,y,z), (r,z) or (r).', desc = 'This field and the angle field describe the position of a component. The value of "numdist" and "numang" uniquely define whether the coordinates are cartesian (numdist=3,num_angle=0), cylindrical (numdist=2,numang=1), or spherical (numdist=1,numang=2). For absolute position, the origin is the scattering center (where a perfectly aligned sample would be) with the z-axis pointing downstream and the y-axis pointing gravitationally up. For a relative position the NXtranslation is taken into account before the NXorientation. The axes are right-handed and orthonormal.', minOccur = 0, enums = [],  attrs = distances_attrs)

angles_attrs = {}
angles_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degree', 'radian'])
NXtranslation_elems['angles'] = NXelem_type(name='angles', types = [5],  dims = ['numobj', 'numang'], info = '(azimuthal_angle,polar_angle) or (azumiuthal_angle). See the documentation in "distances" field.', desc = '', minOccur = 0, enums = [],  attrs = angles_attrs)
NXtranslation_groups = {}
NXtranslation_groups['NXgeometry'] = NXgroup_type(nxclass='NXgeometry', minOccur = 0,  maxOccur = 1, attrs = None, elems = None, groups = None)

group_types['NXtranslation'] = NXgroup_type(nxclass = 'NXtranslation', minOccur= 0, maxOccur= 0, attrs= NXtranslation_attrs, elems= NXtranslation_elems, groups= NXtranslation_groups)


NXuser_attrs = {}
NXuser_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = [])

NXuser_elems = {}

affiliation_attrs = {}
NXuser_elems['affiliation'] = NXelem_type(name='affiliation', types = [4],  dims = [-1], info = 'Affiliation of user', desc = '', minOccur = 0, enums = [],  attrs = affiliation_attrs)

role_attrs = {}
NXuser_elems['role'] = NXelem_type(name='role', types = [4],  dims = [-1], info = 'role of user responsible for this entry', desc = 'Suggested roles are "local_contact", "principal_investigator", and "proposer"', minOccur = 0, enums = [],  attrs = role_attrs)

name_attrs = {}
NXuser_elems['name'] = NXelem_type(name='name', types = [4],  dims = [-1], info = 'Name of user responsible for this entry', desc = '', minOccur = 0, enums = [],  attrs = name_attrs)

address_attrs = {}
NXuser_elems['address'] = NXelem_type(name='address', types = [4],  dims = [-1], info = 'Address of user', desc = '', minOccur = 0, enums = [],  attrs = address_attrs)

telephone_number_attrs = {}
NXuser_elems['telephone_number'] = NXelem_type(name='telephone_number', types = [4],  dims = [-1], info = 'Telephone number of user', desc = '', minOccur = 0, enums = [],  attrs = telephone_number_attrs)

fax_number_attrs = {}
NXuser_elems['fax_number'] = NXelem_type(name='fax_number', types = [4],  dims = [-1], info = 'Fax number of user', desc = '', minOccur = 0, enums = [],  attrs = fax_number_attrs)

facility_user_id_attrs = {}
NXuser_elems['facility_user_id'] = NXelem_type(name='facility_user_id', types = [4],  dims = [-1], info = 'facility based unique identifier for this person e.g. their identification code on the facility address/contact database', desc = '', minOccur = 0, enums = [],  attrs = facility_user_id_attrs)

email_attrs = {}
NXuser_elems['email'] = NXelem_type(name='email', types = [4],  dims = [-1], info = 'Email of user', desc = '', minOccur = 0, enums = [],  attrs = email_attrs)
NXuser_groups = {}

group_types['NXuser'] = NXgroup_type(nxclass = 'NXuser', minOccur= 0, maxOccur= 0, attrs= NXuser_attrs, elems= NXuser_elems, groups= NXuser_groups)


NXvelocity_selector_attrs = {}
NXvelocity_selector_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = ['selector_name'])

NXvelocity_selector_elems = {}

rotation_speed_attrs = {}
rotation_speed_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['rpm'])
NXvelocity_selector_elems['rotation_speed'] = NXelem_type(name='rotation_speed', types = [5],  dims = [1], info = 'selector rotation speed', desc = '', minOccur = 1, enums = [' \n      ?\n   '],  attrs = rotation_speed_attrs)

wavelength_spread_attrs = {}
NXvelocity_selector_elems['wavelength_spread'] = NXelem_type(name='wavelength_spread', types = [5],  dims = [1], info = '% deviation FWHM /Wavelength', desc = '', minOccur = 1, enums = [' \n     ?\n   '],  attrs = wavelength_spread_attrs)

height_attrs = {}
height_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXvelocity_selector_elems['height'] = NXelem_type(name='height', types = [5],  dims = [1], info = 'input beam height', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = height_attrs)

twist_attrs = {}
twist_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degrees'])
NXvelocity_selector_elems['twist'] = NXelem_type(name='twist', types = [5],  dims = [1], info = 'twist angle along axis', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = twist_attrs)

width_attrs = {}
width_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXvelocity_selector_elems['width'] = NXelem_type(name='width', types = [5],  dims = [1], info = 'input beam width', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = width_attrs)

length_attrs = {}
length_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXvelocity_selector_elems['length'] = NXelem_type(name='length', types = [5],  dims = [1], info = 'rotor length', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = length_attrs)

num_attrs = {}
NXvelocity_selector_elems['num'] = NXelem_type(name='num', types = [24],  dims = [1], info = 'number of spokes/lamella', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = num_attrs)

radius_attrs = {}
radius_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXvelocity_selector_elems['radius'] = NXelem_type(name='radius', types = [5],  dims = [1], info = 'radieus at beam centre', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = radius_attrs)

wavelength_attrs = {}
wavelength_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['nm'])
NXvelocity_selector_elems['wavelength'] = NXelem_type(name='wavelength', types = [5],  dims = [1], info = 'wavelength', desc = '', minOccur = 1, enums = [' \n      \n   '],  attrs = wavelength_attrs)

spwidth_attrs = {}
spwidth_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXvelocity_selector_elems['spwidth'] = NXelem_type(name='spwidth', types = [5],  dims = [1], info = 'spoke width at beam centre', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = spwidth_attrs)

table_attrs = {}
table_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['degrees'])
NXvelocity_selector_elems['table'] = NXelem_type(name='table', types = [5],  dims = [1], info = 'offset vertical angle', desc = '', minOccur = 1, enums = ['\n      ?\n   '],  attrs = table_attrs)

type_attrs = {}
NXvelocity_selector_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = 'Vselect type', desc = '', minOccur = 0, enums = [],  attrs = type_attrs)
NXvelocity_selector_groups = {}

group_types['NXvelocity_selector'] = NXgroup_type(nxclass = 'NXvelocity_selector', minOccur= 0, maxOccur= 0, attrs= NXvelocity_selector_attrs, elems= NXvelocity_selector_elems, groups= NXvelocity_selector_groups)


NXcollimator_attrs = {}
NXcollimator_attrs['name'] = NXattr_type(name='name', info = 'Name of collimator', desc = '', minOccur = 1, enums = [])

NXcollimator_elems = {}

distance_attrs = {}
distance_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['m'])
NXcollimator_elems['distance'] = NXelem_type(name='distance', types = [5],  dims = [1], info = 'Distance from sample', desc = '', minOccur = 0, enums = [],  attrs = distance_attrs)

length_attrs = {}
length_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcollimator_elems['length'] = NXelem_type(name='length', types = [5],  dims = [1], info = 'Length of collimator', desc = '', minOccur = 0, enums = [],  attrs = length_attrs)

radius_attrs = {}
radius_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcollimator_elems['radius'] = NXelem_type(name='radius', types = [5],  dims = [1], info = 'Radius of aperture (if circular)', desc = '', minOccur = 0, enums = [],  attrs = radius_attrs)

horizontal_aperture_attrs = {}
horizontal_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcollimator_elems['horizontal_aperture'] = NXelem_type(name='horizontal_aperture', types = [5],  dims = [1], info = 'Horizontal aperture (if rectangular)', desc = '', minOccur = 0, enums = [],  attrs = horizontal_aperture_attrs)

vertical_aperture_attrs = {}
vertical_aperture_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['cm'])
NXcollimator_elems['vertical_aperture'] = NXelem_type(name='vertical_aperture', types = [5],  dims = [1], info = 'Vertical aperture (if rectangular)', desc = '', minOccur = 0, enums = [],  attrs = vertical_aperture_attrs)

soller_angle_attrs = {}
soller_angle_attrs['units'] = NXattr_type(name='units', info = '', desc = '', minOccur = 1, enums = ['minutes'])
NXcollimator_elems['soller_angle'] = NXelem_type(name='soller_angle', types = [5],  dims = [1], info = 'Angular divergence of Soller collimator', desc = '', minOccur = 0, enums = [],  attrs = soller_angle_attrs)

type_attrs = {}
NXcollimator_elems['type'] = NXelem_type(name='type', types = [4],  dims = [-1], info = '', desc = '', minOccur = 0, enums = ['"Soller"', '"..."'],  attrs = type_attrs)
NXcollimator_groups = {}

group_types['NXcollimator'] = NXgroup_type(nxclass = 'NXcollimator', minOccur= 0, maxOccur= 0, attrs= NXcollimator_attrs, elems= NXcollimator_elems, groups= NXcollimator_groups)


NXprocess_attrs = {}
NXprocess_attrs['name'] = NXattr_type(name='name', info = '', desc = '', minOccur = 1, enums = [])

NXprocess_elems = {}
NXprocess_groups = {}
NXprocess_groups['NXnote'] = NXgroup_type(nxclass='NXnote', minOccur = 1,  maxOccur = 2, attrs = None, elems = None, groups = None)

group_types['NXprocess'] = NXgroup_type(nxclass = 'NXprocess', minOccur= 0, maxOccur= 0, attrs= NXprocess_attrs, elems= NXprocess_elems, groups= NXprocess_groups)


