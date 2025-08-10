import csv
import pandas as pd
import plotly.graph_objs as go
import mplfinance as mpf

# Read data from CSV file
with open('Data.csv') as file:
    reader = csv.reader(file, delimiter=",", quotechar='"')
    next(reader, None)  # Skip the header
    data_read = [row for row in reader]

# Initialize containers for data
d_open = [[], [], [], []]
d_high = [[], [], [], []]
d_low = [[], [], [], []]
d_close = [[], [], [], []]

# Map dates to indices
# ISU ="465716"
# day= (int(ISU[:-2])%27)+1
# day_=day+2
dates = {'17/09/18': 0, '17/10/18': 1, "19/11/18": 2, '17/12/18': 3}
inv_dates = {v: k for k, v in dates.items()}

# Process data
for raw in data_read:
    id = dates[raw[2]]
    d_open[id].append(float(raw[4]))
    d_high[id].append(float(raw[5]))
    d_low[id].append(float(raw[6]))
    d_close[id].append(float(raw[7]))

# Prepare DataFrame for CSV
output_data = []


for i in range(0,4):
    cur_date = inv_dates[i]
    for raw in data_read:
        if raw[2] == cur_date:
            output_data.append([pd.Timestamp(cur_date).date(), raw[4], raw[5], raw[6], raw[7]])
    # avg_open = sum(d_open[i]) / len(d_open[i]) if d_open[i] else None
    # avg_high = sum(d_high[i]) / len(d_high[i]) if d_high[i] else None
    # avg_low = sum(d_low[i]) / len(d_low[i]) if d_low[i] else None
    # avg_close = sum(d_close[i]) / len(d_close[i]) if d_close[i] else None
    # output_data.append([cur_date, avg_open, avg_high, avg_low, avg_close])

df = pd.DataFrame(output_data, columns=[
                  'Date', 'Open', 'High', 'Low', 'Close'])

# Save to CSV
df.to_csv('box_plot_data.csv', index=False)
print("Data saved as box_plot_data.csv")
# mpf.plot(output_data, type ="candle", title="candle stick chart",
#          ylabel="price", style="yahoo")
# Create the box plot
# fig = go.Figure()
# for i in range(4):
#     cur_date = inv_dates[i]
#     n = cur_date + ' - Open'
#     fig.add_trace(go.Box(y=d_open[i], name=n))

#     n = cur_date + ' - High'
#     fig.add_trace(go.Box(y=d_high[i], name=n))

#     n = cur_date + ' - Low'
#     fig.add_trace(go.Box(y=d_low[i], name=n))

#     n = cur_date + ' - Close'
#     fig.add_trace(go.Box(y=d_close[i], name=n))

# fig.update_layout(legend=dict(yanchor="top", orientation="h", y=1.2))
# fig.update_xaxes(tickangle=90, title_standoff=25)

# # Save the figure as an image
# fig.write_image("chart.png", format="png")
# print("Chart saved as chart.png")
