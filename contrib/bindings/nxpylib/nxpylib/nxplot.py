import grace_np


class NXplot:
	def __init__(self, gp=None):
		# do initializing
		self.gp = gp # grace plot
		
	def plotData(self, nxdatapoints, axispoints):
		# plot the data via grace
		# nxdatapoints must contain the nxdata as array 
		# axispoints must contain axis data as array of same size
		
		if self.gp == None:
			self.gp = grace_np.GraceProcess()
		
		self.gp.command('s0 on')
		self.gp.command('s0 symbol 1')
		self.gp.command('s0 symbol 1')
		self.gp.command('s0 symbol size 0.3')
		self.gp.command('s0 symbol fill pattern 1')
		

		# check if length are same size, if not, crop smaller list
		if not len(nxdatapoints) == len(axispoints):
			if len(nxdatapoints) > len(axispoints):
				nxdatapoints = nxdatapoints[0:len(axispoints)-1]
			else:
				axispoints = axispoints[0:len(nxdatapoints)-1]
				
		for i in range(len(nxdatapoints)): 
			self.gp.command('g0.s0 point %f,%f' % (axispoints[i],nxdatapoints[i]))
			self.gp.command('autoscale')
			self.gp.command('redraw')
		self.gp.command('autoscale')
		self.gp.command('redraw')


