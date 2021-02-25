
import serial
import sys


def errorRate(lines):
    errors = 0
    for i in range(0, len(lines)-1):
        diff = int(lines[i+1]) - int(lines[i]) -1

        if diff > 0:
            errors += diff 
    
    print ('we got this much errors: ', errors , ' for this many lines: ', len(lines))
    
    error_rate = 100.0 * (float(errors) / float(len(lines)))

    return "error rate: " + str(error_rate)

def write_to_log(file_name, arr):
    file_name = file_name + '.log'

    error_rate = errorRate(arr)
    with open(file_name, 'w') as f:
        for item in arr:
            f.write("%s\n" % item)
        f.write(error_rate)
        

if __name__ == "__main__":
    
    arguments = len(sys.argv) - 1
    if arguments != 1: 
        print('invalid args')
        exit(1)

    log_name = sys.argv[1]
    arr= []

    try:

        with serial.Serial('/dev/ttyUSB0', 115200, timeout=7) as ser:
            while True:
                #line = ser.readline(eol='\r')
                #line = ser.read_until(b'\r')
                line = ser.readline().decode('utf-8')


                if len(line) > 2:
                    print(line)
                    arr.append(line)

    except KeyboardInterrupt:
        write_to_log(log_name, arr)
        sys.exit()

