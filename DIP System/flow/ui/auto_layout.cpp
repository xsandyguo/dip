/*
 * Copyright (c) 1997, 2019. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact xsandyguo@qq.com if you need additional information or have any
 * questions.
 *
 */

#include "flow/ui/auto_layout.h"

#include <map>
#include <algorithm>

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>

#include "base/debug.h"
#include "base/scoped_ptr.h"

void AutoLayoutor::Layout(){
    if(!figures_.size()){
        return;
    }

    ogdf::Graph graph;
    std::map<ogdf::NodeElement*, IFigure*> nodeMap;
    std::map<IFigure*, ogdf::NodeElement*> figureMap;
    
    for (IDrawing::FigureCollection::const_iterator iter = figures_.begin();
        iter != figures_.end(); ++iter){
        IFigure* figure = *iter;
        if(dynamic_cast<IConnectionFigure*>(figure)){
            continue;
        }
        
        // find block        
        ogdf::NodeElement* node = graph.newNode();
        nodeMap[node]           = figure;        
        figureMap[figure]       = node;
    }

    for (IDrawing::FigureCollection::const_iterator iter = figures_.begin();
        iter != figures_.end(); ++iter){
        IConnectionFigure* figure = dynamic_cast<IConnectionFigure*>(*iter);
        if(!figure){
            continue;
        }

        // find edge
        ogdf::NodeElement* node1 = figureMap.find(figure->GetConnectStart()->Owner())->second;
        ogdf::NodeElement* node2 = figureMap.find(figure->GetConnectEnd()->Owner())->second;
        graph.newEdge(node1, node2);
    }
    // node size
    ogdf::GraphAttributes graph_attr(graph,
                              ogdf::GraphAttributes::nodeGraphics | 
                              ogdf::GraphAttributes::edgeGraphics |
                              ogdf::GraphAttributes::nodeLabel | 
                              ogdf::GraphAttributes::nodeColor |
                              ogdf::GraphAttributes::edgeColor | 
                              ogdf::GraphAttributes::edgeStyle |
                              ogdf::GraphAttributes::nodeStyle | 
                              ogdf::GraphAttributes::nodeTemplate);
    ogdf::NodeElement* node;
    forall_nodes(node, graph){
        IFigure* item = nodeMap[node];
        graph_attr.width(node)  = item->GetDrawingArea().Width;
        graph_attr.height(node) = item->GetDrawingArea().Height;
    }

    // compute layout
    ogdf::SugiyamaLayout layout;
    ogdf::FastHierarchyLayout* ohl(new ogdf::FastHierarchyLayout);
    ohl->layerDistance(30);
    ohl->nodeDistance(25);
    layout.setLayout(ohl);
    layout.call(graph_attr);

    // update node position
    forall_nodes(node, graph){
        double x = graph_attr.x(node);
        double y = graph_attr.y(node);
        IFigure* item = nodeMap[node];
        item->SetPosition(Point((int)y, (int)x));
    }
}


void AlignLayoutor::Layout(){
    AlignOrient align(align_);

    int max_edge = align == AO_Left || align == AO_Top ? INT_MAX : INT_MIN;

    std::for_each(figures_.begin(), figures_.end(), [&](IFigure* figure){
        int pos = 0;
        switch(align){
        case AO_Left:
            max_edge = std::min<int>(max_edge, figure->GetBounds().GetLeft());
            break;
        case AO_Right:
            max_edge = std::max<int>(max_edge, figure->GetBounds().GetRight());
            break;
        case AO_Top:
            max_edge = std::min<int>(max_edge, figure->GetBounds().GetTop());
            break;
        case AO_Bottom:
            max_edge = std::max<int>(max_edge, figure->GetBounds().GetBottom());
            break;
        default:
            ASSERT1(false);
        }
    });


    std::for_each(figures_.begin(), figures_.end(), [&](IFigure* figure){
        Point location;
        figure->GetBounds().GetLocation(&location);
        switch(align){
        case AO_Left:
            location.X = max_edge;
            break;
        case AO_Right:
            location.X = max_edge - figure->GetBounds().Width;
            break;
        case AO_Top:
            location.Y = max_edge;
            break;
        case AO_Bottom:
            location.Y = max_edge - figure->GetBounds().Height;
            break;
        default:
            ASSERT1(false);
        }

        figure->SetPosition(location);
    });
}