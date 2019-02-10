#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec 18 22:04:48 2018

@author: wang
"""

import os
import re
install_path = os.getcwd()

re_location = re.compile(r'Node (\d+) connect: ([\d+ ]*)')
re_packet = re.compile(r'Time: (\d+), Packet ID: (\d+), Forward Packet from:(\d+) to (\d+)')

out = []
last_time = {}
sourceID = {}
with open(os.path.join(install_path,'out.txt'),'r') as f:
    for line in f:
        matchobj_1 = re_packet.match(line)
        matchobj_2 = re_location.match(line)
        if matchobj_1:
            time, packet_id, from_id, to_id = int(matchobj_1.group(1)), int(matchobj_1.group(2)), int(matchobj_1.group(3)), int(matchobj_1.group(4))
            if packet_id not in last_time:
                start = '$ns at {}s \"$source{} start\"'.format(time,sourceID[(from_id,to_id)])
                stop = '$ns at {}s \"$source{} stop\"'.format(time+1,sourceID[(from_id,to_id)])
                last_time[packet_id] = time
            else:
                start = '$ns at {}s \"$source{} start\"'.format(last_time[packet_id]+1,sourceID[(from_id,to_id)])
                stop = '$ns at {}s \"$source{} stop\"'.format(last_time[packet_id]+2,sourceID[(from_id,to_id)])
                last_time[packet_id] += 1
            out.append((start,stop))
        if matchobj_2:
            from_id, to_ids = int(matchobj_2.group(1)), matchobj_2.group(2)
            for to_id in to_ids.split():
                source = (from_id, int(to_id))
                if source not in sourceID:
                    sourceID[source] = len(sourceID)

with open(os.path.join(install_path,'result.txt'),'w') as f:
    for item in sourceID:
        from_id, to_id = item
        f.write('$ns simplex-link $n{} $n{} 2Mb 2s DropTail\n'.format(from_id,to_id))
    f.write('\n')
        
    for item in sourceID:
        from_id, to_id = item
        f.write('set source{} [attach-expoo-traffic $n{} $sink{} 1015 4k {}]\n'.format(sourceID[(from_id,to_id)], from_id, to_id, packet_id%5))
    f.write('\n')
    
    for start, stop in out:
        f.write(start + '\n')
        f.write(stop + '\n')
        f.write('\n')