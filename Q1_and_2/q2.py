from mininet.net import Mininet
from mininet.node import OVSController, OVSKernelSwitch
from mininet.link import TCLink
from mininet.topo import Topo
from mininet.cli import CLI
import time
import os

class TopoA3Q2(Topo):
    def build(self):
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')

        h1 = self.addHost('h1', ip='10.1.1.2/24')
        h2 = self.addHost('h2', ip='10.1.1.3/24')
        h3 = self.addHost('h3', ip='10.0.0.4/24')
        h4 = self.addHost('h4', ip='10.0.0.5/24')
        h5 = self.addHost('h5', ip='10.0.0.6/24')
        h6 = self.addHost('h6', ip='10.0.0.7/24')
        h7 = self.addHost('h7', ip='10.0.0.8/24')
        h8 = self.addHost('h8', ip='10.0.0.9/24')
        h9 = self.addHost('h9', ip='172.16.10.10')

        self.addLink(h1, h9, delay='5ms')
        self.addLink(h2, h9, delay='5ms')

        self.addLink(h9, s1, delay='5ms')
        self.addLink(h4, s2, delay='5ms')
        self.addLink(h3, s2, delay='5ms')
        self.addLink(h6, s3, delay='5ms')
        self.addLink(h5, s3, delay='5ms')
        self.addLink(h7, s4, delay='5ms')
        self.addLink(h8, s4, delay='5ms')
        
        self.addLink(s1, s2, delay='7ms')
        self.addLink(s3, s2, delay='7ms')  
        self.addLink(s1, s3, delay='7ms')  
        self.addLink(s1, s4, delay='7ms')  
        self.addLink(s4, s3, delay='7ms')   
        

def run():
    net = Mininet(topo=TopoA3Q2(), controller=OVSController, switch=OVSKernelSwitch, link=TCLink)
    net.start()

    h1 = net.get('h1')
    h2 = net.get('h2')
    h3 = net.get('h3')
    h4 = net.get('h4')
    h5 = net.get('h5')
    h6 = net.get('h6')
    h7 = net.get('h7')
    h8 = net.get('h8')
    h9 = net.get('h9')
    
    for sw in ['s1', 's2', 's3', 's4']:
        sw_obj = net.get(sw)
        sw_obj.cmd('ovs-vsctl set Bridge {} stp_enable=true'.format(sw))

    h1.setIP('10.1.1.2/24', intf='h1-eth0')  # H1 subnet
    h2.setIP('10.2.2.2/24', intf='h2-eth0')  # H2 subnet

    h9.setIP('10.1.1.1/24', intf='h9-eth0')  # Interface to H1
    h9.setIP('10.2.2.1/24', intf='h9-eth1')  # Interface to H2
    h9.setIP('10.0.0.10/24', intf='h9-eth2')  # Public interface

    h1.cmd('ip route add default via 10.1.1.1')
    h2.cmd('ip route add default via 10.2.2.1')

    h9.cmd('sysctl -w net.ipv4.ip_forward=1')
    
    h3.cmd('ip route add default via 10.0.0.10')
    h4.cmd('ip route add default via 10.0.0.10')
    h5.cmd('ip route add default via 10.0.0.10')
    h6.cmd('ip route add default via 10.0.0.10')
    h7.cmd('ip route add default via 10.0.0.10')
    h8.cmd('ip route add default via 10.0.0.10')

    h9.cmd('iptables -t nat -F')
    h9.cmd('iptables -t nat -A POSTROUTING -s 10.1.1.0/24 -o h9-eth2 -j MASQUERADE')
    h9.cmd('iptables -t nat -A POSTROUTING -s 10.2.2.0/24 -o h9-eth2 -j MASQUERADE')


    print("Routing and STP in progress...")
    time.sleep(30)
    h9.cmd('iptables -t nat -L -n -v > nat_rules.txt')

    CLI(net)

    # IPERF3 TESTS

    # print("Iperf3 test 1 begins...")

    # h1.cmd('iperf3 -s > h1_server_output.txt &')
    # h6.cmd('iperf3 -c 10.1.1.2 -t 120 > h6_client_output.txt')

    # print("Iperf3 test 2 begins...")


    # h8.cmd('iperf3 -s > h8_server_output.txt &')
    # h2.cmd('iperf3 -c 10.0.0.9 -t 120 > h2_client_output.txt')


    net.stop()

if __name__ == '__main__':
    run()
