import * as React from "react"
import { Link } from "gatsby"
import * as echarts from 'echarts/core';
import { TooltipComponent, TooltipComponentOption } from 'echarts/components';
import { TreeChart, TreeSeriesOption } from 'echarts/charts';
import { CanvasRenderer } from 'echarts/renderers';
import ReactECharts from 'echarts-for-react';


const App: React.FC = () => {
  const data = require('../tree.json');
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

export default App;