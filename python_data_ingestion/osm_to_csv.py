import  os
import re
from lxml import etree
import utm



# Serves a nice little csv up on a platter for cpp to make a bunch of buildings
# Not efficient at all, should rlngeall
def main():

    data = etree.parse("example_map_chicago.osm")

    # Buildings seem to follow this pattern
    buildings = data.xpath("way[tag[contains(@k, 'height')]]")

    # For better cpp float acc we are going to subtract first utms
    first = True
    xsub = 0
    ysub = 0
    with open("render_instructions_chi.txt", "w+") as f:
        for element in buildings:
            height = element.xpath("tag[contains(@k, 'height')]/@v")[0]

            # Dumb edge cases
            if height == "default":
                height = '10'

            if "'" in height and '"' in height:
                ft = float(height.split("'")[0])
                inch = float(height.replace('"', '').split("'")[-1])
                height = str(.3048*ft + .0254*inch)

            f.write(f'{height}\n')

            for nd_ref in element.xpath('nd/@ref'):
                lat=data.xpath(f"node[@id='{nd_ref}']/@lat")[0]
                lon=data.xpath(f"node[@id='{nd_ref}']/@lon")[0]
                u = utm.from_latlon(float(lat), float(lon))
                if first:
                    xsub = u[0]
                    ysub = u[1]
                    x = 0.0
                    y = 0.0
                    first = False
                else:
                    x = u[0] - xsub
                    y = u[1] - ysub

                f.write(f'{x}\n')
                f.write(f'{y}\n')
            f.write('END\n')


if __name__ == "__main__":
    main()

