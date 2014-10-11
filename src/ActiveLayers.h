#ifndef ACTIVELAYERS_H
#define ACTIVELAYERS_H

#include "Stencils.h"
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <Eigen/Core>

class PenaltyGroup;
class Mesh;
struct SimulationState;

class PenaltyGroupComparator
{
public:
	bool operator()(const PenaltyGroup *first, const PenaltyGroup *second) const;
};

class ActiveLayers
{
public:
	ActiveLayers(double eta, double baseDt, double baseStiffness, double terminationTime, bool verbose = false);	
	~ActiveLayers();

	void addVFStencil(VertexFaceStencil stencil);	
	void addEEStencil(EdgeEdgeStencil stencil);

	bool runOneIteration(const Mesh &m, SimulationState &initialState);
	bool collisionDetection(const Eigen::VectorXd &qend, const Mesh &m, std::set<VertexFaceStencil> &vfDetected, std::set<EdgeEdgeStencil> &eeDetected);

private:
	ActiveLayers(const ActiveLayers &other);
	ActiveLayers &operator=(const ActiveLayers &other);

	void addGroups(int maxdepth);
	bool step(SimulationState &s);
	void rollback();
	double layerDepth(int layer);
	double VFStencilThickness(VertexFaceStencil stencil);	
	double EEStencilThickness(EdgeEdgeStencil stencil);

	double eta_;
	double baseDt_;
	double baseStiffness_;
	double termTime_;
	
	int deepestLayer_;

	std::vector<PenaltyGroup *> groups_;
	std::priority_queue<PenaltyGroup *, std::vector<PenaltyGroup *>, PenaltyGroupComparator> groupQueue_;
	std::map<VertexFaceStencil, int> vfdepth_;
	std::map<EdgeEdgeStencil, int> eedepth_;

	bool verbose_;
};

#endif
