#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, OVSSwitch
from mininet.log import setLogLevel, info
from mininet.link import TCLink
import time
import os

class TopoA3Q1B(Topo):

    def build(self):

        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')

        h1 = self.addHost('h1', ip='10.0.0.2/24')
        h2 = self.addHost('h2', ip='10.0.0.4/24')
        h3 = self.addHost('h3', ip='10.0.0.3/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        
        self.addLink(s1, s2, delay='7ms')
        self.addLink(s2, s3, delay='7ms')
        self.addLink(s3, s4, delay='7ms')
        self.addLink(s4, s1, delay='7ms')
        self.addLink(s1, s3, delay='7ms')
        
        self.addLink(h1, s1, delay='5ms')
        self.addLink(h2, s1, delay='5ms')
        self.addLink(h3, s2, delay='5ms')
        self.addLink(h4, s2, delay='5ms')
        self.addLink(h5, s3, delay='5ms')
        self.addLink(h6, s3, delay='5ms')
        self.addLink(h7, s4, delay='5ms')
        self.addLink(h8, s4, delay='5ms')

def main():
    import time
    topo = TopoA3Q1B()
    net = Mininet(topo=topo, link=TCLink, switch=OVSSwitch, controller=Controller)
    net.start()

    
    for switch in net.switches:
        switch.cmd('ovs-vsctl set bridge %s stp_enable=true' % switch.name)
    
    print("STP is converging...")
    # Wait for STP to converge
    time.sleep(30)

    h3 = net.get('h3')

    print("Starting tcpdump on h3...")
    h3.cmd('tcpdump -i h3-eth0 -w h3_to_h1_stp.pcap &')

    time.sleep(5)

    print("\nPinging h1 from h3...")
    result = h3.cmd('ping -c 1 10.0.0.2')
    print(result)

    time.sleep(2)

    h3.cmd('kill %tcpdump')
    print("\nSaved packet capture to h3_to_h1.pcap")

    # Test 1
    
    # result = net.get("h3").cmd(f"ping -c 3 {net.get('h1').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h3").cmd(f"ping -c 3 {net.get('h1').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h3").cmd(f"ping -c 3 {net.get('h1').IP()}")
    # print(result)
    

    # Test 2

    # result = net.get("h5").cmd(f"ping -c 3 {net.get('h7').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h5").cmd(f"ping -c 3 {net.get('h7').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h5").cmd(f"ping -c 3 {net.get('h7').IP()}")
    # print(result)

    # Test 3
    
    # result = net.get("h8").cmd(f"ping -c 3 {net.get('h2').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h8").cmd(f"ping -c 3 {net.get('h2').IP()}")
    # print(result)
    # time.sleep(30)

    # result = net.get("h8").cmd(f"ping -c 3 {net.get('h2').IP()}")
    # print(result)

    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    main()