namespace AutoBattler
{
    public class Wizzard : Hero
    {
        public Wizzard(string name, int hp, int defense, int damage)
            : base(name, hp, defense, damage) { }

        public override void SpecialAbility(Hero target)
        {
            if (target == null) return;
            int rawDamage = Damage * 2;
            target.HP -= rawDamage; 
            if (target.HP < 0) target.HP = 0;
        }
    }
}
