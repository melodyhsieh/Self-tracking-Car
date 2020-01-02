import sys, tty, BT

ser = BT.bluetooth()
port = input('PC bluetooth port name: ')
port = port if port != '0' else '/dev/cu.HC-05-SPPDev'
ser.do_connect(port)

tty.setcbreak(sys.stdin)
print ('START!!')

'''
while True:
	print (sys.stdin.read(1))
'''

pickup = 0

while True:
	key = sys.stdin.read(1)  # key captures the key-code 
	# based on the input we do something - in this case print something
	print ('you pressed {}'.format(key))
	ser.SerialWrite(key)
	if key == 'p':
		print ('end session')
		sys.exit()
	elif key == 'f':
		if pickup == 0:
			pickup = 1
			print ('Magnet ON')
		else:
			pickup = 0
			print ('Magnet OFF')