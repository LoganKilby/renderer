internal void
GatherPoints()
{
    u32 headStatus = HeadStatusOK(laser_map.head_status[iProfile]);
    if(!headStatus)
    {
        // no points to draw for this laser head
    }
    
    // Stores profile points in "fXY" and "fFillXY"
    LoadProfilePoints(iProfile, bFirstTime[0], iRemoveSawnRegion);
}