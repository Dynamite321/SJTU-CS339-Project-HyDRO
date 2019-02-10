#The source file for NS2 Warm Up
#################################################################################
# Create a simulator object
set ns [new Simulator]

# Define colors
$ns color 2 Blue
$ns color 1 Red
$ns color 3 Green
$ns color 0 Black
$ns color 4 Yellow
$ns color 5 Purple

# Open the output files for recording
set f0 [open out0.tr w]
set f1 [open out1.tr w]
set f2 [open out2.tr w]
set f3 [open out3.tr w]

# Open a file for the nam trace data
set nf [open out.nam w]
$ns namtrace-all $nf

# Define the 'finish' procedure
proc finish {} {
	global f0 f1 f2 f3 ns nf
	#Close the output files
	close $f0
	close $f1
	close $f2
        close $f3
	close $nf
	exec nam out.nam &
	#Call xgraph to display the results
	exec xgraph out0.tr out1.tr out2.tr out3.tr -geometry 800x600 &
	exit 0
	}

# Define the 'record' procedure
proc record {} {
	global sink1 sink8 f0 f1
	#Get an instance of the simulator
	set ns [Simulator instance]
	#Set the time after which the procedure should be called again
	set time 1.0
	#How many bytes have been received by the traffic sinks?
	set bw0 [$sink1 set bytes_]
	set bw1 [$sink8 set bytes_]
	#Get the current time
	set now [$ns now]
	#Calculate the bandwidth (in MBit/s) and write it to the files
	puts $f0 "$now [expr $bw0/$time*8/1000000]"
	puts $f1 "$now [expr $bw1/$time*8/1000000]"
	#Reset the bytes_ values on the traffic sinks
	#Re-schedule the procedure
	$ns at [expr $now+$time] "record"
}

# Define the attach-expoo-traffic procedure
proc attach-expoo-traffic { node sink size rate color } {
	#Get an instance of the simulator
	set ns [Simulator instance]

	#Create a UDP agent and attach it to the node
	set source [new Agent/UDP]
	$ns attach-agent $node $source
	$source set fid_ $color

	#Create an Expoo traffic agent and set its configuration parameters
	set traffic [new Application/Traffic/CBR]
	$traffic set packetSize_ $size
        $traffic set rate_ $rate
        $traffic set interval_ 2s

	#Attach traffic source to the traffic generator
	$traffic attach-agent $source

	#Connect the source and the sink
	$ns connect $source $sink
	
	return $traffic
}

# Nodes definition
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]

# Nodes connection
$ns simplex-link $n0 $n2 100kb 1s DropTail
$ns simplex-link $n0 $n3 100kb 1s DropTail
$ns simplex-link $n0 $n6 100kb 1s DropTail
$ns simplex-link $n0 $n8 100kb 1s DropTail
$ns simplex-link $n1 $n9 100kb 1s DropTail
$ns simplex-link $n2 $n1 100kb 1s DropTail
$ns simplex-link $n2 $n6 100kb 1s DropTail
$ns simplex-link $n2 $n8 100kb 1s DropTail
$ns simplex-link $n3 $n2 100kb 1s DropTail
$ns simplex-link $n3 $n4 100kb 1s DropTail
$ns simplex-link $n4 $n1 100kb 1s DropTail
$ns simplex-link $n4 $n2 100kb 1s DropTail
$ns simplex-link $n5 $n3 100kb 1s DropTail
$ns simplex-link $n5 $n4 100kb 1s DropTail
$ns simplex-link $n6 $n1 100kb 1s DropTail
$ns simplex-link $n6 $n8 100kb 1s DropTail
$ns simplex-link $n7 $n0 100kb 1s DropTail
$ns simplex-link $n7 $n3 100kb 1s DropTail
$ns simplex-link $n7 $n5 100kb 1s DropTail
$ns simplex-link $n8 $n1 100kb 1s DropTail
$ns simplex-link $n8 $n9 100kb 1s DropTail


# Nodes position for nam
$ns simplex-link-op $n7 $n5 orient right-up
$ns simplex-link-op $n7 $n3 orient right
$ns simplex-link-op $n7 $n0 orient right-down
$ns simplex-link-op $n5 $n3 orient down
$ns simplex-link-op $n0 $n3 orient up
$ns simplex-link-op $n3 $n4 orient right
$ns simplex-link-op $n3 $n2 orient right-down
$ns simplex-link-op $n5 $n4 orient right-down
$ns simplex-link-op $n0 $n3 orient up
$ns simplex-link-op $n0 $n2 orient right
$ns simplex-link-op $n0 $n8 orient right-down
$ns simplex-link-op $n0 $n6 orient down
$ns simplex-link-op $n4 $n2 orient down
$ns simplex-link-op $n4 $n1 orient right-down
$ns simplex-link-op $n2 $n1 orient right
$ns simplex-link-op $n2 $n8 orient down
$ns simplex-link-op $n2 $n6 orient left-down
$ns simplex-link-op $n8 $n1 orient right-up
$ns simplex-link-op $n1 $n9 orient right


set sink0 [new Agent/LossMonitor]
set sink1 [new Agent/LossMonitor]
set sink2 [new Agent/LossMonitor]
set sink3 [new Agent/LossMonitor]
set sink4 [new Agent/LossMonitor]
set sink5 [new Agent/LossMonitor]
set sink6 [new Agent/LossMonitor]
set sink7 [new Agent/LossMonitor]
set sink8 [new Agent/LossMonitor]
set sink9 [new Agent/LossMonitor]

$ns attach-agent $n0 $sink0
$ns attach-agent $n1 $sink1
$ns attach-agent $n2 $sink2
$ns attach-agent $n3 $sink3
$ns attach-agent $n4 $sink4
$ns attach-agent $n5 $sink5
$ns attach-agent $n6 $sink6
$ns attach-agent $n7 $sink7
$ns attach-agent $n8 $sink8
$ns attach-agent $n9 $sink9

# 2500 0.01 5 ; 1400 0.01 4 ; 600 0.01 3 ; 400 0.01 2 ; 333 0.01 1 ; 200 0.01 6
set source0 [attach-expoo-traffic $n0 $sink3 1015 4k 1]
set source1 [attach-expoo-traffic $n3 $sink2 1015 4k 1]
set source2 [attach-expoo-traffic $n2 $sink6 1015 4k 1]
set source3 [attach-expoo-traffic $n6 $sink8 1015 4k 1]
set source4 [attach-expoo-traffic $n8 $sink1 1015 4k 1]
set source5 [attach-expoo-traffic $n1 $sink9 1015 4k 1]
set source6 [attach-expoo-traffic $n7 $sink0 1015 4k 2]
set source7 [attach-expoo-traffic $n0 $sink3 1015 4k 2]
set source8 [attach-expoo-traffic $n3 $sink4 1015 4k 2]
set source9 [attach-expoo-traffic $n4 $sink2 1015 4k 2]
set source10 [attach-expoo-traffic $n2 $sink8 1015 4k 2]
set source11 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source12 [attach-expoo-traffic $n2 $sink8 1015 4k 3]
set source13 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source14 [attach-expoo-traffic $n1 $sink9 1015 4k 4]
set source15 [attach-expoo-traffic $n0 $sink8 1015 4k 5]
set source16 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source17 [attach-expoo-traffic $n1 $sink9 1015 4k 0]
set source18 [attach-expoo-traffic $n3 $sink2 1015 4k 1]
set source19 [attach-expoo-traffic $n2 $sink8 1015 4k 1]
set source20 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source21 [attach-expoo-traffic $n2 $sink8 1015 4k 2]
set source22 [attach-expoo-traffic $n8 $sink1 1015 4k 2]
set source23 [attach-expoo-traffic $n1 $sink9 1015 4k 2]
set source24 [attach-expoo-traffic $n0 $sink3 1015 4k 3]
set source25 [attach-expoo-traffic $n3 $sink4 1015 4k 3]
set source26 [attach-expoo-traffic $n4 $sink2 1015 4k 3]
set source27 [attach-expoo-traffic $n2 $sink8 1015 4k 3]
set source28 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source29 [attach-expoo-traffic $n2 $sink6 1015 4k 4]
set source30 [attach-expoo-traffic $n6 $sink8 1015 4k 4]
set source31 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source32 [attach-expoo-traffic $n4 $sink2 1015 4k 5]
set source33 [attach-expoo-traffic $n2 $sink6 1015 4k 5]
set source34 [attach-expoo-traffic $n6 $sink8 1015 4k 5]
set source35 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source36 [attach-expoo-traffic $n4 $sink2 1015 4k 0]
set source37 [attach-expoo-traffic $n2 $sink6 1015 4k 0]
set source38 [attach-expoo-traffic $n6 $sink8 1015 4k 0]
set source39 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source40 [attach-expoo-traffic $n2 $sink6 1015 4k 1]
set source41 [attach-expoo-traffic $n6 $sink8 1015 4k 1]
set source42 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source43 [attach-expoo-traffic $n1 $sink9 1015 4k 2]
set source44 [attach-expoo-traffic $n5 $sink3 1015 4k 3]
set source45 [attach-expoo-traffic $n3 $sink4 1015 4k 3]
set source46 [attach-expoo-traffic $n4 $sink2 1015 4k 3]
set source47 [attach-expoo-traffic $n2 $sink8 1015 4k 3]
set source48 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source49 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source50 [attach-expoo-traffic $n4 $sink2 1015 4k 5]
set source51 [attach-expoo-traffic $n2 $sink8 1015 4k 5]
set source52 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source53 [attach-expoo-traffic $n7 $sink3 1015 4k 0]
set source54 [attach-expoo-traffic $n3 $sink4 1015 4k 0]
set source55 [attach-expoo-traffic $n4 $sink2 1015 4k 0]
set source56 [attach-expoo-traffic $n2 $sink8 1015 4k 0]
set source57 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source58 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source59 [attach-expoo-traffic $n1 $sink9 1015 4k 2]
set source60 [attach-expoo-traffic $n0 $sink8 1015 4k 3]
set source61 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source62 [attach-expoo-traffic $n7 $sink3 1015 4k 4]
set source63 [attach-expoo-traffic $n3 $sink4 1015 4k 4]
set source64 [attach-expoo-traffic $n4 $sink2 1015 4k 4]
set source65 [attach-expoo-traffic $n2 $sink8 1015 4k 4]
set source66 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source67 [attach-expoo-traffic $n7 $sink3 1015 4k 5]
set source68 [attach-expoo-traffic $n3 $sink4 1015 4k 5]
set source69 [attach-expoo-traffic $n4 $sink2 1015 4k 5]
set source70 [attach-expoo-traffic $n2 $sink8 1015 4k 5]
set source71 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source72 [attach-expoo-traffic $n7 $sink3 1015 4k 0]
set source73 [attach-expoo-traffic $n3 $sink4 1015 4k 0]
set source74 [attach-expoo-traffic $n4 $sink2 1015 4k 0]
set source75 [attach-expoo-traffic $n2 $sink8 1015 4k 0]
set source76 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source77 [attach-expoo-traffic $n4 $sink2 1015 4k 1]
set source78 [attach-expoo-traffic $n2 $sink6 1015 4k 1]
set source79 [attach-expoo-traffic $n6 $sink8 1015 4k 1]
set source80 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source81 [attach-expoo-traffic $n0 $sink8 1015 4k 2]
set source82 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source83 [attach-expoo-traffic $n2 $sink6 1015 4k 3]
set source84 [attach-expoo-traffic $n6 $sink8 1015 4k 3]
set source85 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source86 [attach-expoo-traffic $n7 $sink3 1015 4k 4]
set source87 [attach-expoo-traffic $n3 $sink4 1015 4k 4]
set source88 [attach-expoo-traffic $n4 $sink2 1015 4k 4]
set source89 [attach-expoo-traffic $n2 $sink6 1015 4k 4]
set source90 [attach-expoo-traffic $n6 $sink8 1015 4k 4]
set source91 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source92 [attach-expoo-traffic $n2 $sink6 1015 4k 5]
set source93 [attach-expoo-traffic $n6 $sink8 1015 4k 5]
set source94 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source95 [attach-expoo-traffic $n6 $sink8 1015 4k 0]
set source96 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source97 [attach-expoo-traffic $n0 $sink8 1015 4k 1]
set source98 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source99 [attach-expoo-traffic $n3 $sink4 1015 4k 2]
set source100 [attach-expoo-traffic $n4 $sink2 1015 4k 2]
set source101 [attach-expoo-traffic $n2 $sink8 1015 4k 2]
set source102 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source103 [attach-expoo-traffic $n5 $sink3 1015 4k 3]
set source104 [attach-expoo-traffic $n3 $sink4 1015 4k 3]
set source105 [attach-expoo-traffic $n4 $sink2 1015 4k 3]
set source106 [attach-expoo-traffic $n2 $sink8 1015 4k 3]
set source107 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source108 [attach-expoo-traffic $n6 $sink8 1015 4k 4]
set source109 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source110 [attach-expoo-traffic $n7 $sink3 1015 4k 5]
set source111 [attach-expoo-traffic $n3 $sink4 1015 4k 5]
set source112 [attach-expoo-traffic $n4 $sink2 1015 4k 5]
set source113 [attach-expoo-traffic $n2 $sink8 1015 4k 5]
set source114 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source115 [attach-expoo-traffic $n2 $sink8 1015 4k 0]
set source116 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source117 [attach-expoo-traffic $n3 $sink4 1015 4k 1]
set source118 [attach-expoo-traffic $n4 $sink2 1015 4k 1]
set source119 [attach-expoo-traffic $n2 $sink8 1015 4k 1]
set source120 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source121 [attach-expoo-traffic $n2 $sink8 1015 4k 2]
set source122 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source123 [attach-expoo-traffic $n1 $sink9 1015 4k 3]
set source124 [attach-expoo-traffic $n3 $sink4 1015 4k 4]
set source125 [attach-expoo-traffic $n4 $sink2 1015 4k 4]
set source126 [attach-expoo-traffic $n2 $sink8 1015 4k 4]
set source127 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source128 [attach-expoo-traffic $n2 $sink8 1015 4k 5]
set source129 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source130 [attach-expoo-traffic $n6 $sink8 1015 4k 0]
set source131 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source132 [attach-expoo-traffic $n5 $sink3 1015 4k 1]
set source133 [attach-expoo-traffic $n3 $sink4 1015 4k 1]
set source134 [attach-expoo-traffic $n4 $sink2 1015 4k 1]
set source135 [attach-expoo-traffic $n2 $sink8 1015 4k 1]
set source136 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source137 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source138 [attach-expoo-traffic $n6 $sink8 1015 4k 3]
set source139 [attach-expoo-traffic $n8 $sink9 1015 4k 3]
set source140 [attach-expoo-traffic $n7 $sink3 1015 4k 4]
set source141 [attach-expoo-traffic $n3 $sink4 1015 4k 4]
set source142 [attach-expoo-traffic $n4 $sink2 1015 4k 4]
set source143 [attach-expoo-traffic $n2 $sink6 1015 4k 4]
set source144 [attach-expoo-traffic $n6 $sink8 1015 4k 4]
set source145 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source146 [attach-expoo-traffic $n5 $sink3 1015 4k 5]
set source147 [attach-expoo-traffic $n3 $sink4 1015 4k 5]
set source148 [attach-expoo-traffic $n4 $sink2 1015 4k 5]
set source149 [attach-expoo-traffic $n2 $sink6 1015 4k 5]
set source150 [attach-expoo-traffic $n6 $sink8 1015 4k 5]
set source151 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source152 [attach-expoo-traffic $n1 $sink9 1015 4k 0]
set source153 [attach-expoo-traffic $n7 $sink3 1015 4k 1]
set source154 [attach-expoo-traffic $n3 $sink4 1015 4k 1]
set source155 [attach-expoo-traffic $n4 $sink2 1015 4k 1]
set source156 [attach-expoo-traffic $n2 $sink6 1015 4k 1]
set source157 [attach-expoo-traffic $n6 $sink8 1015 4k 1]
set source158 [attach-expoo-traffic $n8 $sink9 1015 4k 1]
set source159 [attach-expoo-traffic $n4 $sink2 1015 4k 2]
set source160 [attach-expoo-traffic $n2 $sink6 1015 4k 2]
set source161 [attach-expoo-traffic $n6 $sink8 1015 4k 2]
set source162 [attach-expoo-traffic $n8 $sink9 1015 4k 2]
set source163 [attach-expoo-traffic $n1 $sink9 1015 4k 3]
set source164 [attach-expoo-traffic $n0 $sink8 1015 4k 4]
set source165 [attach-expoo-traffic $n8 $sink9 1015 4k 4]
set source166 [attach-expoo-traffic $n2 $sink6 1015 4k 5]
set source167 [attach-expoo-traffic $n6 $sink8 1015 4k 5]
set source168 [attach-expoo-traffic $n8 $sink9 1015 4k 5]
set source169 [attach-expoo-traffic $n6 $sink8 1015 4k 0]
set source170 [attach-expoo-traffic $n8 $sink9 1015 4k 0]
set source171 [attach-expoo-traffic $n6 $sink8 1015 4k 1]
set source172 [attach-expoo-traffic $n8 $sink9 1015 4k 1]


$ns at 0.0 "record"

$ns at 0s "$source0 start"
$ns at 1s "$source0 stop"

$ns at 1s "$source1 start"
$ns at 2s "$source1 stop"

$ns at 2s "$source2 start"
$ns at 3s "$source2 stop"

$ns at 3s "$source3 start"
$ns at 4s "$source3 stop"

$ns at 4s "$source4 start"
$ns at 5s "$source4 stop"

$ns at 5s "$source5 start"
$ns at 6s "$source5 stop"

$ns at 2s "$source6 start"
$ns at 3s "$source6 stop"

$ns at 3s "$source7 start"
$ns at 4s "$source7 stop"

$ns at 4s "$source8 start"
$ns at 5s "$source8 stop"

$ns at 5s "$source9 start"
$ns at 6s "$source9 stop"

$ns at 6s "$source10 start"
$ns at 7s "$source10 stop"

$ns at 7s "$source11 start"
$ns at 8s "$source11 stop"

$ns at 14s "$source12 start"
$ns at 15s "$source12 stop"

$ns at 15s "$source13 start"
$ns at 16s "$source13 stop"

$ns at 15s "$source14 start"
$ns at 16s "$source14 stop"

$ns at 21s "$source15 start"
$ns at 22s "$source15 stop"

$ns at 22s "$source16 start"
$ns at 23s "$source16 stop"

$ns at 26s "$source17 start"
$ns at 27s "$source17 stop"

$ns at 49s "$source18 start"
$ns at 50s "$source18 stop"

$ns at 50s "$source19 start"
$ns at 51s "$source19 stop"

$ns at 51s "$source20 start"
$ns at 52s "$source20 stop"

$ns at 50s "$source21 start"
$ns at 51s "$source21 stop"

$ns at 51s "$source22 start"
$ns at 52s "$source22 stop"

$ns at 52s "$source23 start"
$ns at 53s "$source23 stop"

$ns at 56s "$source24 start"
$ns at 57s "$source24 stop"

$ns at 57s "$source25 start"
$ns at 58s "$source25 stop"

$ns at 58s "$source26 start"
$ns at 59s "$source26 stop"

$ns at 59s "$source27 start"
$ns at 60s "$source27 stop"

$ns at 60s "$source28 start"
$ns at 61s "$source28 stop"

$ns at 61s "$source29 start"
$ns at 62s "$source29 stop"

$ns at 62s "$source30 start"
$ns at 63s "$source30 stop"

$ns at 63s "$source31 start"
$ns at 64s "$source31 stop"

$ns at 65s "$source32 start"
$ns at 66s "$source32 stop"

$ns at 66s "$source33 start"
$ns at 67s "$source33 stop"

$ns at 67s "$source34 start"
$ns at 68s "$source34 stop"

$ns at 68s "$source35 start"
$ns at 69s "$source35 stop"

$ns at 66s "$source36 start"
$ns at 67s "$source36 stop"

$ns at 67s "$source37 start"
$ns at 68s "$source37 stop"

$ns at 68s "$source38 start"
$ns at 69s "$source38 stop"

$ns at 69s "$source39 start"
$ns at 70s "$source39 stop"

$ns at 73s "$source40 start"
$ns at 74s "$source40 stop"

$ns at 74s "$source41 start"
$ns at 75s "$source41 stop"

$ns at 75s "$source42 start"
$ns at 76s "$source42 stop"

$ns at 74s "$source43 start"
$ns at 75s "$source43 stop"

$ns at 76s "$source44 start"
$ns at 77s "$source44 stop"

$ns at 77s "$source45 start"
$ns at 78s "$source45 stop"

$ns at 78s "$source46 start"
$ns at 79s "$source46 stop"

$ns at 79s "$source47 start"
$ns at 80s "$source47 stop"

$ns at 80s "$source48 start"
$ns at 81s "$source48 stop"

$ns at 79s "$source49 start"
$ns at 80s "$source49 stop"

$ns at 90s "$source50 start"
$ns at 91s "$source50 stop"

$ns at 91s "$source51 start"
$ns at 92s "$source51 stop"

$ns at 92s "$source52 start"
$ns at 93s "$source52 stop"

$ns at 101s "$source53 start"
$ns at 102s "$source53 stop"

$ns at 102s "$source54 start"
$ns at 103s "$source54 stop"

$ns at 103s "$source55 start"
$ns at 104s "$source55 stop"

$ns at 104s "$source56 start"
$ns at 105s "$source56 stop"

$ns at 105s "$source57 start"
$ns at 106s "$source57 stop"

$ns at 104s "$source58 start"
$ns at 105s "$source58 stop"

$ns at 125s "$source59 start"
$ns at 126s "$source59 stop"

$ns at 126s "$source60 start"
$ns at 127s "$source60 stop"

$ns at 127s "$source61 start"
$ns at 128s "$source61 stop"

$ns at 140s "$source62 start"
$ns at 141s "$source62 stop"

$ns at 141s "$source63 start"
$ns at 142s "$source63 stop"

$ns at 142s "$source64 start"
$ns at 143s "$source64 stop"

$ns at 143s "$source65 start"
$ns at 144s "$source65 stop"

$ns at 144s "$source66 start"
$ns at 145s "$source66 stop"

$ns at 143s "$source67 start"
$ns at 144s "$source67 stop"

$ns at 144s "$source68 start"
$ns at 145s "$source68 stop"

$ns at 145s "$source69 start"
$ns at 146s "$source69 stop"

$ns at 146s "$source70 start"
$ns at 147s "$source70 stop"

$ns at 147s "$source71 start"
$ns at 148s "$source71 stop"

$ns at 144s "$source72 start"
$ns at 145s "$source72 stop"

$ns at 145s "$source73 start"
$ns at 146s "$source73 stop"

$ns at 146s "$source74 start"
$ns at 147s "$source74 stop"

$ns at 147s "$source75 start"
$ns at 148s "$source75 stop"

$ns at 148s "$source76 start"
$ns at 149s "$source76 stop"

$ns at 147s "$source77 start"
$ns at 148s "$source77 stop"

$ns at 148s "$source78 start"
$ns at 149s "$source78 stop"

$ns at 149s "$source79 start"
$ns at 150s "$source79 stop"

$ns at 150s "$source80 start"
$ns at 151s "$source80 stop"

$ns at 161s "$source81 start"
$ns at 162s "$source81 stop"

$ns at 162s "$source82 start"
$ns at 163s "$source82 stop"

$ns at 168s "$source83 start"
$ns at 169s "$source83 stop"

$ns at 169s "$source84 start"
$ns at 170s "$source84 stop"

$ns at 170s "$source85 start"
$ns at 171s "$source85 stop"

$ns at 176s "$source86 start"
$ns at 177s "$source86 stop"

$ns at 177s "$source87 start"
$ns at 178s "$source87 stop"

$ns at 178s "$source88 start"
$ns at 179s "$source88 stop"

$ns at 179s "$source89 start"
$ns at 180s "$source89 stop"

$ns at 180s "$source90 start"
$ns at 181s "$source90 stop"

$ns at 181s "$source91 start"
$ns at 182s "$source91 stop"

$ns at 181s "$source92 start"
$ns at 182s "$source92 stop"

$ns at 182s "$source93 start"
$ns at 183s "$source93 stop"

$ns at 183s "$source94 start"
$ns at 184s "$source94 stop"

$ns at 185s "$source95 start"
$ns at 186s "$source95 stop"

$ns at 186s "$source96 start"
$ns at 187s "$source96 stop"

$ns at 191s "$source97 start"
$ns at 192s "$source97 stop"

$ns at 192s "$source98 start"
$ns at 193s "$source98 stop"

$ns at 201s "$source99 start"
$ns at 202s "$source99 stop"

$ns at 202s "$source100 start"
$ns at 203s "$source100 stop"

$ns at 203s "$source101 start"
$ns at 204s "$source101 stop"

$ns at 204s "$source102 start"
$ns at 205s "$source102 stop"

$ns at 202s "$source103 start"
$ns at 203s "$source103 stop"

$ns at 203s "$source104 start"
$ns at 204s "$source104 stop"

$ns at 204s "$source105 start"
$ns at 205s "$source105 stop"

$ns at 205s "$source106 start"
$ns at 206s "$source106 stop"

$ns at 206s "$source107 start"
$ns at 207s "$source107 stop"

$ns at 205s "$source108 start"
$ns at 206s "$source108 stop"

$ns at 206s "$source109 start"
$ns at 207s "$source109 stop"

$ns at 214s "$source110 start"
$ns at 215s "$source110 stop"

$ns at 215s "$source111 start"
$ns at 216s "$source111 stop"

$ns at 216s "$source112 start"
$ns at 217s "$source112 stop"

$ns at 217s "$source113 start"
$ns at 218s "$source113 stop"

$ns at 218s "$source114 start"
$ns at 219s "$source114 stop"

$ns at 223s "$source115 start"
$ns at 224s "$source115 stop"

$ns at 224s "$source116 start"
$ns at 225s "$source116 stop"

$ns at 225s "$source117 start"
$ns at 226s "$source117 stop"

$ns at 226s "$source118 start"
$ns at 227s "$source118 stop"

$ns at 227s "$source119 start"
$ns at 228s "$source119 stop"

$ns at 228s "$source120 start"
$ns at 229s "$source120 stop"

$ns at 228s "$source121 start"
$ns at 229s "$source121 stop"

$ns at 229s "$source122 start"
$ns at 230s "$source122 stop"

$ns at 232s "$source123 start"
$ns at 233s "$source123 stop"

$ns at 235s "$source124 start"
$ns at 236s "$source124 stop"

$ns at 236s "$source125 start"
$ns at 237s "$source125 stop"

$ns at 237s "$source126 start"
$ns at 238s "$source126 stop"

$ns at 238s "$source127 start"
$ns at 239s "$source127 stop"

$ns at 250s "$source128 start"
$ns at 251s "$source128 stop"

$ns at 251s "$source129 start"
$ns at 252s "$source129 stop"

$ns at 254s "$source130 start"
$ns at 255s "$source130 stop"

$ns at 255s "$source131 start"
$ns at 256s "$source131 stop"

$ns at 258s "$source132 start"
$ns at 259s "$source132 stop"

$ns at 259s "$source133 start"
$ns at 260s "$source133 stop"

$ns at 260s "$source134 start"
$ns at 261s "$source134 stop"

$ns at 261s "$source135 start"
$ns at 262s "$source135 stop"

$ns at 262s "$source136 start"
$ns at 263s "$source136 stop"

$ns at 259s "$source137 start"
$ns at 260s "$source137 stop"

$ns at 260s "$source138 start"
$ns at 261s "$source138 stop"

$ns at 261s "$source139 start"
$ns at 262s "$source139 stop"

$ns at 262s "$source140 start"
$ns at 263s "$source140 stop"

$ns at 263s "$source141 start"
$ns at 264s "$source141 stop"

$ns at 264s "$source142 start"
$ns at 265s "$source142 stop"

$ns at 265s "$source143 start"
$ns at 266s "$source143 stop"

$ns at 266s "$source144 start"
$ns at 267s "$source144 stop"

$ns at 267s "$source145 start"
$ns at 268s "$source145 stop"

$ns at 268s "$source146 start"
$ns at 269s "$source146 stop"

$ns at 269s "$source147 start"
$ns at 270s "$source147 stop"

$ns at 270s "$source148 start"
$ns at 271s "$source148 stop"

$ns at 271s "$source149 start"
$ns at 272s "$source149 stop"

$ns at 272s "$source150 start"
$ns at 273s "$source150 stop"

$ns at 273s "$source151 start"
$ns at 274s "$source151 stop"

$ns at 269s "$source152 start"
$ns at 270s "$source152 stop"

$ns at 270s "$source153 start"
$ns at 271s "$source153 stop"

$ns at 271s "$source154 start"
$ns at 272s "$source154 stop"

$ns at 272s "$source155 start"
$ns at 273s "$source155 stop"

$ns at 273s "$source156 start"
$ns at 274s "$source156 stop"

$ns at 274s "$source157 start"
$ns at 275s "$source157 stop"

$ns at 275s "$source158 start"
$ns at 276s "$source158 stop"

$ns at 275s "$source159 start"
$ns at 276s "$source159 stop"

$ns at 276s "$source160 start"
$ns at 277s "$source160 stop"

$ns at 277s "$source161 start"
$ns at 278s "$source161 stop"

$ns at 278s "$source162 start"
$ns at 279s "$source162 stop"

$ns at 280s "$source163 start"
$ns at 281s "$source163 stop"

$ns at 282s "$source164 start"
$ns at 283s "$source164 stop"

$ns at 283s "$source165 start"
$ns at 284s "$source165 stop"

$ns at 286s "$source166 start"
$ns at 287s "$source166 stop"

$ns at 287s "$source167 start"
$ns at 288s "$source167 stop"

$ns at 288s "$source168 start"
$ns at 289s "$source168 stop"

$ns at 290s "$source169 start"
$ns at 291s "$source169 stop"

$ns at 291s "$source170 start"
$ns at 292s "$source170 stop"

$ns at 294s "$source171 start"
$ns at 295s "$source171 stop"

$ns at 295s "$source172 start"
$ns at 296s "$source172 stop"


$ns at 296.0 "finish"


$ns run

