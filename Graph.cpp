#include "Graph.hpp"
using namespace std;

// NodeInfo -----------------------------------------------------------------------------------------------------------------------------------

NodeInfo::NodeInfo() {
    preActivationValue = 0;
    bias = 0;
    activationFunction = identity;
    activationDerivative = identity;
    activate();
    delta = 0;
}

NodeInfo::NodeInfo(string activationFunction, double value, double bias) {
    this->activationFunction = getActivationFunction(activationFunction);
    this->activationDerivative = getActivationDerivative(activationFunction);
    this->preActivationValue = value;
    this->activate();
    this->bias = bias;
    this->delta = 0;
}

double NodeInfo::activate() {
    postActivationValue = activationFunction(preActivationValue);
    return postActivationValue;
}

double NodeInfo::derive() {
    return activationDerivative(preActivationValue);
}

bool NodeInfo::operator==(const NodeInfo& other) {
    return (this->preActivationValue == other.preActivationValue) && 
           (this->postActivationValue == other.postActivationValue) && 
           (this->activationFunction == other.activationFunction) &&
           (this->bias == other.bias) &&
           (this->activationDerivative == other.activationDerivative);
}

std::ostream& operator<<(std::ostream& out, const NodeInfo& n) {
    out << "bias: " << n.bias << 
           " preActivationValue: " << n.preActivationValue << 
           " postActivationValue: " << n.postActivationValue << 
           " activationFunction: " << getActivationIdentifier(n.activationFunction) <<
           " activationDerivative: " << getActivationIdentifier(n.activationDerivative) << endl;
    return out;
}

// Connection -----------------------------------------------------------------------------------------------------------------------------------

Connection::Connection() {
    this->source = -1;
    this->dest = -1;
    this->weight = 0;
    this->delta = 0;
}

Connection::Connection(int source, int dest, double weight) {
    this->source = source;
    this->dest = dest;
    this->weight = weight;
    this->delta = 0;
}

bool Connection::operator<(const Connection& other) {
    return this->dest < other.dest;
}

bool Connection::operator==(const Connection& other) {
    return (this->dest == other.dest) && 
           (this->source == other.source) && 
           (this->weight == other.weight);
}

std::ostream& operator<<(std::ostream& out, const Connection& c) {
    out << "source: " << c.source << 
           " dest: " << c.dest << 
           " weight: " << c.weight << endl;
    return out;
}



// ------------------------ YOU DO NOT NEED TO MODIFY THE CODE ABOVE (but feel free to explore it) --------------------------------------




// Graph -----------------------------------------------------------------------------------------------------------------------------------

// STUDENT TODO: IMPLEMENT
void Graph::updateNode(int id, NodeInfo n) {
    if (id < 0 || id >= nodes.size()) {
        cout << "Attempting to update node with id: " << id << " but node does not exist" << endl;
        return;
    }
    if (nodes[id] != nullptr) {
        delete nodes[id];
    }
    nodes[id] = new NodeInfo(n);
}

// STUDENT TODO: IMPLEMENT
NodeInfo* Graph::getNode(int id) const {
    if (id < 0 || id >= nodes.size()) {
        return nullptr;
    }
    return nodes[id];
}

// STUDENT TODO: IMPLEMENT
void Graph::updateConnection(int v, int u, double w) {
    if (v < 0 || v >= nodes.size()) {
        cerr << "Attempting to update connection between " << v << " and " << u << " with weight " << w << " but " << v << " does not exist" << endl;
        exit(1);
    }
    if (u < 0 || u >= nodes.size()) {
        cerr << "Attempting to update connection between " << v << " and " << u << " with weight " << w << " but " << u << " does not exist" << endl;
        exit(1);
    }
    adjacencyList[v][u] = Connection(v, u, w);
}

// STUDENT TODO: IMPLEMENT
void Graph::clear() {
    for (auto& node : nodes) {
        if (node != nullptr) {
            delete node;
            node = nullptr;
        }
    }
    nodes.clear();
    adjacencyList.clear();
    size = 0;
}




// ------------------------ YOU DO NOT NEED TO MODIFY THE REMAINING CODE (but please feel free to explore it!) --------------------------------------





Graph::Graph() {
    this->size = 0;
}

Graph::Graph(int size) {
    resize(size);
}

Graph::Graph(const Graph& other) {
    this->size = other.size;
    this->adjacencyList = other.adjacencyList;
    for (int i = 0; i < other.nodes.size(); i++) {
        nodes.push_back(new NodeInfo(*other.nodes.at(i)));
    }
}

Graph& Graph::operator=(const Graph& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    this->size = other.size;
    this->adjacencyList = other.adjacencyList;
    for (int i = 0; i < other.nodes.size(); i++) {
        nodes.push_back(new NodeInfo(*other.nodes.at(i)));
    }
    return *this;
}

Graph::~Graph() {
    clear();
}

AdjList& Graph::getAdjacencyList() {
    return adjacencyList;
}

ostream& operator<<(ostream& out, const Graph& g) {
    // output as dot format for graph visualization
    out << "digraph G {" << endl;
    for (int i = 0; i < g.adjacencyList.size(); i++) {
        for (auto j = g.adjacencyList.at(i).begin(); j != g.adjacencyList.at(i).end(); j++) {
            out << "\t" << i << " -> " << j->second.dest << "[label=\"" << j->second.weight << "\"]" << endl;
        }
    }
    out << "}" << endl;
    for (int i = 0; i < g.nodes.size(); i++) {
        string end = "\n";
        if (i == g.nodes.size()-1) {
            end = "";
        }
        out << "node " << i 
            << ": (z=" << g.nodes.at(i)->preActivationValue << "\t"\
            << ", a=" << g.nodes.at(i)->postActivationValue << "\t"
            << ", bias=" << g.nodes.at(i)->bias << "\t"
            << ", activation=" << getActivationIdentifier(g.nodes.at(i)->activationFunction) << ")" << end;
    }

    // take the console output starting from digraph G {...} (to the last bracket) and paste it here: 
    // from test_neuralnet.txt:
    //https://dreampuf.github.io/GraphvizOnline/?engine=dot#digraph%20G%20%7B%0A%090%20-%3E%2011%5Blabel%3D%221.34082%22%5D%0A%090%20-%3E%2010%5Blabel%3D%22-0.223228%22%5D%0A%090%20-%3E%209%5Blabel%3D%22-0.169643%22%5D%0A%091%20-%3E%2011%5Blabel%3D%221.18517%22%5D%0A%091%20-%3E%2010%5Blabel%3D%220.194131%22%5D%0A%091%20-%3E%209%5Blabel%3D%22-0.0402987%22%5D%0A%092%20-%3E%2011%5Blabel%3D%22-0.156924%22%5D%0A%092%20-%3E%2010%5Blabel%3D%220.989135%22%5D%0A%092%20-%3E%209%5Blabel%3D%220.565788%22%5D%0A%093%20-%3E%2011%5Blabel%3D%22-0.46132%22%5D%0A%093%20-%3E%2010%5Blabel%3D%220.642295%22%5D%0A%093%20-%3E%209%5Blabel%3D%220.762242%22%5D%0A%094%20-%3E%2011%5Blabel%3D%220.633375%22%5D%0A%094%20-%3E%2010%5Blabel%3D%220.297137%22%5D%0A%094%20-%3E%209%5Blabel%3D%22-0.268914%22%5D%0A%095%20-%3E%2011%5Blabel%3D%220.549267%22%5D%0A%095%20-%3E%2010%5Blabel%3D%22-1.35402%22%5D%0A%095%20-%3E%209%5Blabel%3D%22-0.279643%22%5D%0A%096%20-%3E%2011%5Blabel%3D%22-1.03429%22%5D%0A%096%20-%3E%2010%5Blabel%3D%22-1.19832%22%5D%0A%096%20-%3E%209%5Blabel%3D%220.0833546%22%5D%0A%097%20-%3E%2011%5Blabel%3D%22-0.917686%22%5D%0A%097%20-%3E%2010%5Blabel%3D%22-0.0819241%22%5D%0A%097%20-%3E%209%5Blabel%3D%220.740756%22%5D%0A%098%20-%3E%2011%5Blabel%3D%22-0.957671%22%5D%0A%098%20-%3E%2010%5Blabel%3D%22-0.0884205%22%5D%0A%098%20-%3E%209%5Blabel%3D%220.520281%22%5D%0A%099%20-%3E%2012%5Blabel%3D%220.821195%22%5D%0A%0910%20-%3E%2012%5Blabel%3D%22-0.344489%22%5D%0A%0911%20-%3E%2012%5Blabel%3D%22-1.52925%22%5D%0A%7D
    
    //place holder:
        // https://dreampuf.github.io/GraphvizOnline/#digraph%20G%20%7B%0A%0A%20%20subgraph%20cluster_0%20%7B%0A%20%20%20%20style%3Dfilled%3B%0A%20%20%20%20color%3Dlightgrey%3B%0A%20%20%20%20node%20%5Bstyle%3Dfilled%2Ccolor%3Dwhite%5D%3B%0A%20%20%20%20a0%20-%3E%20a1%20-%3E%20a2%20-%3E%20a3%3B%0A%20%20%20%20label%20%3D%20%22process%20%231%22%3B%0A%20%20%7D%0A%0A%20%20subgraph%20cluster_1%20%7B%0A%20%20%20%20node%20%5Bstyle%3Dfilled%5D%3B%0A%20%20%20%20b0%20-%3E%20b1%20-%3E%20b2%20-%3E%20b3%3B%0A%20%20%20%20label%20%3D%20%22process%20%232%22%3B%0A%20%20%20%20color%3Dblue%0A%20%20%7D%0A%20%20start%20-%3E%20a0%3B%0A%20%20start%20-%3E%20b0%3B%0A%20%20a1%20-%3E%20b3%3B%0A%20%20b2%20-%3E%20a3%3B%0A%20%20a3%20-%3E%20a0%3B%0A%20%20a3%20-%3E%20end%3B%0A%20%20b3%20-%3E%20end%3B%0A%0A%20%20start%20%5Bshape%3DMdiamond%5D%3B%0A%20%20end%20%5Bshape%3DMsquare%5D%3B%0A%7D
    return out;
}

void Graph::resize(int size) {
    this->size = size;
    adjacencyList.resize(size);
    for (int i = 0; i < size; i++) {
        nodes.push_back(nullptr);
    }
}

vector<NodeInfo*> Graph::getNodes() const {
    return nodes;
}
