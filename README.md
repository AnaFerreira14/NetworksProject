# Network Project
 
## Introduction

A network composed of nodes with unique identifiers interconnected in a tree is considered. Each node contains a set of content with unique names for each node (content from different nodes can have the same name). A user of a source node who wants to fetch certain content forms a search message containing the destination node identifier, the source node identifier, and the name of the content. This message will be forwarded on the network and will reach the destination, which will respond with a content message, containing the identifier of the node that searched for the content, message destination, its identifier, origin of the message and the content itself (for simplicity, just an indication that the content exists). If the destination does not have the searched content, then it responds with an unavailability message.

Each node maintains a dispatch table with entries, each of which associates a destination node with its neighbor along the only path in the tree there. This table is populated by the messages that cross the network and may be incomplete at any given moment. A node that has a message destined for another node consults its dispatch table. Three situations can occur: 

(1) The node finds an entry for the destination node in the table and the neighbor associated with the destination node is different from the one from which it received the message, or the message originated from the node itself. In this case, the node sends the message to the neighbor read from the dispatch table; 

(2) The node finds an entry for the destination node in the table, but the neighbor associated with this entry is the neighbor where the message was received. Then, the node filters the message, that is, discards it; 

(3) The node does not find an entry for the target node in the table. This time, the node broadcasts the message to all its neighbors, except the one from which it received the message, that is, it sends copies of the message to each of these neighbors.

On the other hand, each time a node receives a message from a neighbor, it introduces an entry into its dispatch table with the association between the originating node of the message and the neighbor through which it was received, to be used in the reverse direction, upon the arrival of another message destined for the origin node of the original message. This procedure is called transparent learning (or automatic learning!).

## Topology of the network

• Each edge of the tree is substantiated in a TCP session, with the two nodes that share the edge being neighbors of each other.

• Each node has only one external neighbor, and may have multiple internal neighbors, or none at all.

• In networks with more than one node, there are exactly two nodes, called anchors, that are external neighbors of each other.

• Anchor nodes have themselves as recovery nodes.

![NetworkExample](https://github.com/AnaFerreira14/NetworksProject/assets/56273313/0f279a97-e415-4512-8d31-b537612f50f9)

## Invocation of the application

In order to run the program, you need to compile the code with the following command:

```make cot```

Then you can invoke the program with the following command:

```cot IP TCP regIP regUDP```

cot - name of the program

IP - IP adress of the machine that has the program

TCP - TCP server of the program server

regIP - IP adress of the node server

regUDP - Port of the node server

## Commands

```join net id``` - Entry of a node into the net network with identifier id.

```djoin net id bootid bootIP bootTCP``` - Entry of a node into the net network with identifier id, which is known to be unique on the network. The application is passed the identifier and contact of a network node, through of the bootid, bootIP and bootTCP parameters, to which the node should connect without interrogating the node server.

```create name``` - A content named name is created.

```delete name``` - The content named name is deleted.

```get dest name``` - Search for content named name in the dest node.

```show topology (st)``` - Shows neighbors' identifiers and contacts internal neighbors, the external neighbor and the recovery neighbor.

```show names (sn)``` - Shows the names of the content present in the node.

```show routing (sr)``` - Shows the node's dispatch table.

```leave``` - Node exits the network.

```exit``` - Finish the program.

