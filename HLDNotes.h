################TECHNIQUES###################################
Distibute traffic: Load Balancer(Private IPs)
Data Replication: Master slave,
Slow Response time: CDN(Content expires after TTL ends),Cache(multiple Cache Servers across different data centers),Non-relational DB
Server Horizontal Scaling: StateLess Web Tier,
AsynchronousCommunication:Message Queue
Database Horizontal Scaling: Sharding
Manage Requests to Server: Rate Limiter(Within API Gateway),
Avoid Server Miss: Consistent Hashing
High Availability and Reliabality: Data Replication in diff DataCenters and datacenters are connected through networks.
DataConsistency:Quorum Consensus
Ability to store big data: Consistent hashing to spread load across servers 
High availability reads:Data replication,Multi-datacenter setup
Highly available writes :Versioning and conflict resolution with Vector clocks
Dataset partition :Consistent Hashing
Incremental scalability:Consistent Hashing
Heterogeneity:Consistent Hashing
Tunable consistency:Quorum consensus
Handling temporary failure:Sloppy quorum and hinted handoff
Handling permanent failures:Merkle tree
Handling data center outage:Cross-datacenter replication 
ServerFailure Detection:Gossible Protocol

#############CONCEPTS######################################
API Gateway: Supports RateLimiting,SSL Termination,Authentication,IP Whitelisting,ServicingStaticContent.
For rate limiter timestamps are stored in RedisCache(InMemory Cache).
REDIS: Remote Dictionary Server is a single threaded event loop,supports INCR and EXPIRE, to avoid RACECONDITION in Redis LUA SCRIPT & SORTED SETS are used.
Consistent Hashing: To avoid uneven load distribution in Consistent Hashing VirtualNodes are being used(Space tradeoff as more space is required to store virtual nodes.)

DataPartition:Consistent Hashing
	Key as data items and Distibute the data in the next servers in clockwise direction like we do in Consistent Hashing.
Data Replication:
	After a key is mapped to a position on the hash ring,walk clockwise from that position and choose the first N servers on the ring to store data copies.With virtusl nodes we only choose unique servers.
DataConsistency:
	W to succeed:Write operation must be acknowledged from W replicas
	R to succeed:Read operation must wait for responses from atleast R replicas
	W+R>N(Number of replicas) : Strong consistency
WritePath(KeyValue Store):
	Write request is persisted on a commit log file
	Data is saved in memory cache
	When the memory cache is full or reaches a predefined threshold,data is flushed to SSTable(Sorted string table is a sorted list of <key,value>pairs)
ReadPath(KeyValue Store):
	If data is in memory cache directly return
	If not then will be retrieved from disk by an efficient way to find out which SSTable contains the key.BLOOM FILTER solves this problem
	SSTable return the result of the data set 
	The result is return to the client.
UniqueIDGeneration:
	1 bit:0
	41 bits:timestamp
	5 bits:datacenter ID
	5 bits:machine ID
	12 bits:sequence number

	Sign bit: 1 bit. It will always be 0. This is reserved for future uses. It can potentially be used to distinguish between signed and unsigned numbers.
	Timestamp: 41 bits. Milliseconds since the epoch or custom epoch Twitter snowflake default epoch 1288834974657. We use equivalent to Nov 04, 2010, 01:42:54 UTC.
	Datacenter ID: 5 bits, which gives us 2^5 = 32 datacenters.
	Machine ID: 5 bits, which gives us 2^5 = 32 machines pem datacenter.
	Sequence number: 12 bits. For every ID generated on that machine/process, the sequence number is incremented by 1. The number is reset to 0 every millisecond	