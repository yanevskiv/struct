#ifndef __GRAPH_H__
#define __GRAPH_H__

struct graph_node {
    void *m_data;
    graph_node_t *m_next;
    graph_node_t *m_prev;
    graph_node_t *m_head;
    graph_node_t *m_tail;
} graph_node_t;

#endif
