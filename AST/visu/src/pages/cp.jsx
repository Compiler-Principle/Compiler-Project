import * as React from "react"
import { Link } from "gatsby"
import * as echarts from 'echarts/core';
import { TooltipComponent, TooltipComponentOption } from 'echarts/components';
import { TreeChart, TreeSeriesOption } from 'echarts/charts';
import { CanvasRenderer } from 'echarts/renderers';
import ReactECharts from 'echarts-for-react';


const Page = () => {
    // const data = require('../components/tree.json');
    const data = {
        "childCnt": 2,
        "children": [
            {
                "childCnt": 2,
                "children": [
                    {
                        "childCnt": 2,
                        "children": [
                            {
                                "childCnt": 0,
                                "children": [],
                                "dataType": 3,
                                "id": 2,
                                "name": "2 typdef:float",
                                "type": 9
                            },
                            {
                                "childCnt": 1,
                                "children": [
                                    {
                                        "childCnt": 0,
                                        "children": [],
                                        "dataType": 0,
                                        "id": 3,
                                        "name": "3 var:a",
                                        "type": 3
                                    }
                                ],
                                "dataType": 0,
                                "id": 4,
                                "name": "4 Var_List",
                                "type": 8
                            }
                        ],
                        "dataType": 0,
                        "id": 5,
                        "name": "5 defi",
                        "type": 7
                    },
                    {
                        "childCnt": 2,
                        "children": [
                            {
                                "childCnt": 0,
                                "children": [],
                                "dataType": 2,
                                "id": 6,
                                "name": "6 typdef:int",
                                "type": 9
                            },
                            {
                                "childCnt": 1,
                                "children": [
                                    {
                                        "childCnt": 0,
                                        "children": [],
                                        "dataType": 0,
                                        "id": 7,
                                        "name": "7 var:b",
                                        "type": 3
                                    }
                                ],
                                "dataType": 0,
                                "id": 8,
                                "name": "8 Var_List",
                                "type": 8
                            }
                        ],
                        "dataType": 0,
                        "id": 9,
                        "name": "9 defi",
                        "type": 7
                    }
                ],
                "dataType": 0,
                "id": 1,
                "name": "1 Def_List",
                "type": 8
            },
            {
                "childCnt": 2,
                "children": [
                    {
                        "childCnt": 2,
                        "children": [
                            {
                                "childCnt": 0,
                                "children": [],
                                "dataType": 2,
                                "id": 11,
                                "name": "11 typdef:int",
                                "type": 9
                            },
                            {
                                "childCnt": 2,
                                "children": [
                                    {
                                        "childCnt": 2,
                                        "children": [
                                            {
                                                "childCnt": 2,
                                                "children": [
                                                    {
                                                        "childCnt": 0,
                                                        "children": [],
                                                        "dataType": 2,
                                                        "id": 13,
                                                        "name": "13 typdef:int",
                                                        "type": 9
                                                    },
                                                    {
                                                        "childCnt": 1,
                                                        "children": [
                                                            {
                                                                "childCnt": 0,
                                                                "children": [],
                                                                "dataType": 0,
                                                                "id": 14,
                                                                "name": "14 var:func_var1",
                                                                "type": 3
                                                            }
                                                        ],
                                                        "dataType": 0,
                                                        "id": 15,
                                                        "name": "15 Var_List",
                                                        "type": 8
                                                    }
                                                ],
                                                "dataType": 0,
                                                "id": 16,
                                                "name": "16 defi",
                                                "type": 7
                                            },
                                            {
                                                "childCnt": 2,
                                                "children": [
                                                    {
                                                        "childCnt": 0,
                                                        "children": [],
                                                        "dataType": 3,
                                                        "id": 17,
                                                        "name": "17 typdef:float",
                                                        "type": 9
                                                    },
                                                    {
                                                        "childCnt": 1,
                                                        "children": [
                                                            {
                                                                "childCnt": 0,
                                                                "children": [],
                                                                "dataType": 0,
                                                                "id": 18,
                                                                "name": "18 var:func_var2",
                                                                "type": 3
                                                            }
                                                        ],
                                                        "dataType": 0,
                                                        "id": 19,
                                                        "name": "19 Var_List",
                                                        "type": 8
                                                    }
                                                ],
                                                "dataType": 0,
                                                "id": 20,
                                                "name": "20 defi",
                                                "type": 7
                                            }
                                        ],
                                        "dataType": 0,
                                        "id": 12,
                                        "name": "12 Def_List",
                                        "type": 8
                                    },
                                    {
                                        "childCnt": 1,
                                        "children": [
                                            {
                                                "childCnt": 1,
                                                "children": [
                                                    {
                                                        "childCnt": 0,
                                                        "children": [],
                                                        "dataType": 2,
                                                        "id": 22,
                                                        "name": "22 const:1 DT_integer",
                                                        "type": 5
                                                    }
                                                ],
                                                "dataType": 0,
                                                "id": 23,
                                                "name": "23 exp:return",
                                                "type": 10
                                            }
                                        ],
                                        "dataType": 0,
                                        "id": 24,
                                        "name": "24 Stmt_List",
                                        "type": 8
                                    }
                                ],
                                "dataType": 0,
                                "id": 25,
                                "name": "25 CBLOCK",
                                "type": 12
                            }
                        ],
                        "dataType": 0,
                        "id": 26,
                        "name": "26 func:t1",
                        "type": 6
                    },
                    {
                        "childCnt": 2,
                        "children": [
                            {
                                "childCnt": 0,
                                "children": [],
                                "dataType": 2,
                                "id": 27,
                                "name": "27 typdef:int",
                                "type": 9
                            },
                            {
                                "childCnt": 2,
                                "children": [
                                    {
                                        "childCnt": 0,
                                        "children": [],
                                        "dataType": 0,
                                        "id": 28,
                                        "name": "28 Def_List",
                                        "type": 8
                                    },
                                    {
                                        "childCnt": 1,
                                        "children": [
                                            {
                                                "childCnt": 1,
                                                "children": [
                                                    {
                                                        "childCnt": 0,
                                                        "children": [],
                                                        "dataType": 2,
                                                        "id": 30,
                                                        "name": "30 const:0 DT_integer",
                                                        "type": 5
                                                    }
                                                ],
                                                "dataType": 0,
                                                "id": 31,
                                                "name": "31 exp:return",
                                                "type": 10
                                            }
                                        ],
                                        "dataType": 0,
                                        "id": 32,
                                        "name": "32 Stmt_List",
                                        "type": 8
                                    }
                                ],
                                "dataType": 0,
                                "id": 33,
                                "name": "33 CBLOCK",
                                "type": 12
                            }
                        ],
                        "dataType": 0,
                        "id": 34,
                        "name": "34 func:main",
                        "type": 6
                    }
                ],
                "dataType": 0,
                "id": 10,
                "name": "10 Fun_List",
                "type": 8
            }
        ],
        "dataType": 0,
        "id": 35,
        "name": "35 Program",
        "type": 0
    };
    console.log(data);


    const opt = {
        tooltip: {
            trigger: 'item',
            triggerOn: 'mousemove'
        },
        series: [
            {
                type: 'tree',

                data: [data],

                top: '5%',
                left: '7%',
                bottom: '1%',
                right: '20%',

                symbolSize: 7,
                orient: 'vertical',
                label: {
                    position: 'right',
                    verticalAlign: 'middle',
                    align: 'left',
                    fontSize: 9
                },

                leaves: {
                    label: {
                        position: 'right',
                        verticalAlign: 'middle',
                        align: 'left'
                    }
                },

                emphasis: {
                    focus: 'descendant'
                },
                initialTreeDepth: -1,
                expandAndCollapse: true,
                animationDuration: 200,
                animationDurationUpdate: 500
            }
        ]
    };


    return (
        <React.Fragment>
            <ReactECharts
                option={opt}
                style={{ height: '500px', width: '100%'}}
            />
        </React.Fragment>
    )
};

export default Page;