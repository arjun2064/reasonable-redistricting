import json
import statistics

with open('PA_VTDs.json') as f:
    data = json.load(f)

    plansToDescription = {
        "REMEDIAL": "Congressional district ID in 2018 enacted remedial plan",
        "GOV": "Congressional district ID in Governor’s counter-proposed plan",
        "TS": "Congressional district ID in Turzai-Scarnati Plan",
        "CD_2011": "Congressional district ID in 2011 enacted congressional map",
        "SEND": "State Senate district ID",
        "HDIST": "State House district ID",
        "538DEM": "FiveThirtyEight Democratic favoring plan",
        "538GOP": "FiveThirtyEight GOP favoring plan",
        "538CMPCT": "FiveThirtyEight plan favoring compactness",
    }

    # republican_votes_district, total_votes_district, percentage_votes
    plansData = { planKey: [[], [], []] for planKey in plansToDescription.keys() }

    # republican_votes_district = []
    # total_votes_district = []

    for i, precinct in enumerate(data["nodes"]):
        for planKey in plansData.keys():
            district_id = int(precinct[planKey])
            republican_votes_district, total_votes_district, _ = plansData[planKey]

            if not (district_id < len(republican_votes_district)):
                republican_votes_district.extend([0] * (district_id - len(republican_votes_district) + 1))
                total_votes_district.extend([0] * (district_id - len(total_votes_district) + 1))


            republican_votes = int(precinct["T16PRESR"])
            democrat_votes = int(precinct["T16PRESD"])
            total_votes = republican_votes + democrat_votes
            republican_votes_district[district_id] += republican_votes
            total_votes_district[district_id] += total_votes


    # no 0 index district
    planToMeanMedian = {}

    for planKey in plansData.keys():
        republican_votes_district, total_votes_district, percentage_votes = plansData[planKey]
        percentage_votes = [
            rep/tot for rep, tot in zip(republican_votes_district[1:], total_votes_district[1:])
        ]

        # calculate mean median
        mean_median = statistics.median(percentage_votes) - statistics.mean(percentage_votes)
        planToMeanMedian[planKey] = mean_median

    with open("./output/PA-mean-median.txt", 'w') as output:
        for k,v in planToMeanMedian.items():
            output.write(f"{k} {v}\n")