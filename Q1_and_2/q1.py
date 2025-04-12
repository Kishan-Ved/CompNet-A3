#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.link import TCLink
from mininet.cli import CLI
from mininet.log import setLogLevel
import time

class TopoA3(Topo):
    def build(self):
        # Add switches
        s1 = self.addSwitch('s1', stp=False)
        s2 = self.addSwitch('s2', stp=False)
        s3 = self.addSwitch('s3', stp=False)
        s4 = self.addSwitch('s4', stp=False)

        # Add hosts with IPs
        h1 = self.addHost('h1', ip='10.0.0.2/24')
        h2 = self.addHost('h2', ip='10.0.0.4/24')
        h3 = self.addHost('h3', ip='10.0.0.3/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')

        # Host-Switch links (5ms delay)
        self.addLink(h1, s1, cls=TCLink, delay='5ms')
        self.addLink(h2, s1, cls=TCLink, delay='5ms')
        self.addLink(h3, s2, cls=TCLink, delay='5ms')
        self.addLink(h4, s2, cls=TCLink, delay='5ms')
        self.addLink(h5, s3, cls=TCLink, delay='5ms')
        self.addLink(h6, s3, cls=TCLink, delay='5ms')
        self.addLink(h7, s4, cls=TCLink, delay='5ms')
        self.addLink(h8, s4, cls=TCLink, delay='5ms')

        # Switch-Switch links (7ms delay)
        self.addLink(s1, s2, cls=TCLink, delay='7ms')
        self.addLink(s2, s3, cls=TCLink, delay='7ms')
        self.addLink(s3, s4, cls=TCLink, delay='7ms')
        self.addLink(s4, s1, cls=TCLink, delay='7ms')
        self.addLink(s1, s3, cls=TCLink, delay='7ms')  # additional diagonal link

def run():
    # Code snippet 1

    # topo = LoopyTopo()
    # net = Mininet(topo=topo, link=TCLink, controller=None, autoSetMacs=True)
    # net.start()

    # print("\nRunning pings that will demonstrate broadcast storm due to network loop (no STP):")
    # print("Try: ping h1 from h3, h5 to h7, h8 to h2")

    # CLI(net)  # You can test pings here
    # net.stop()
    
    



    # Code snippet 2
    
    # topo = TopoA3()
    # net = Mininet(topo=topo, link=TCLink, controller=None, autoSetMacs=True)
    # net.start()

    # h3 = net.get('h3')

    # print("Starting tcpdump on h3...")
    # h3.cmd('tcpdump -i h3-eth0 -w h3_to_h1_2.pcap &')

    # import time
    # time.sleep(5)

    # print("\nPinging h1 from h3...")
    # result = h3.cmd('ping -c 1 10.0.0.2')
    # print(result)

    # time.sleep(2)

    # h3.cmd('kill %tcpdump')
    # print("\nSaved packet capture to h3_to_h1.pcap")

    # CLI(net)
    # net.stop()




    # Code snippet 3

    topo = TopoA3()
    net = Mininet(topo=topo, link=TCLink, controller=None, autoSetMacs=True)
    net.start()

    h1 = net.get('h1')
    h2 = net.get('h2')
    h3 = net.get('h3')
    h4 = net.get('h4')
    h5 = net.get('h5')
    h6 = net.get('h6')
    h7 = net.get('h7')
    h8 = net.get('h8')

    print("\nPinging h2 from h8...")
    result = h8.cmd('ping -c 3 10.0.0.4')
    print(result)

    time.sleep(30)

    print("\nPinging h2 from h8...")
    result = h8.cmd('ping -c 3 10.0.0.4')
    print(result)

    time.sleep(30)

    print("\nPinging h2 from h8...")
    result = h8.cmd('ping -c 3 10.0.0.4')
    print(result)

    # time.sleep(30)

    net.stop()


if __name__ == '__main__':
    setLogLevel('info')
    run()
